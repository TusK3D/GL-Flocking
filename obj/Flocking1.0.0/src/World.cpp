#include "include/World.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Random.h>
#include <ngl/Util.h>

//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.5;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=2.0;

//----------------------------------------------------------------------------------------------------------------------
World::World(
                   int _numBoids, int argc,
                  char *argv[], QWidget *_parent
                  ) : QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent )
{
  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
  // Now set the initial GLWindow attributes to default values
  // Roate is false
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  m_origX=0;
  m_origY=0;
  m_boidUpdateTimer=startTimer(50);
  m_animate=true;
  m_checkBoidBoid =false;
  m_obstacleCount = 0;
  m_centre=0.0,0.0,0.0;
  m_dimensions=80.0,80.0,80.0;
  m_drawBBox = true;
  m_shaderName = "Phong";
  // create vectors for the position and direction
  Parser _parse(argc,argv);
  m_flock=new Flocking();
  m_flock->setBoidCount(_numBoids);
  std::cout<<m_flock->getBoidCount()<<std::endl;
  m_flock->createBoidFlock(m_flock->getBoidCount(), _parse);
}



World::~World()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VBO's and Shaders\n";
  Init->NGLQuit();
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void World::initializeGL()
{
  // create the lights

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
   glEnable(GL_DEPTH_TEST);
   // Now we will create a basic Camera from the graphics library
   // This is a static camera so it only needs to be set once
   // First create Values for the camera position
   ngl::Vec3 from(-15,-15,120);
   ngl::Vec3 to(0,0,0);
   ngl::Vec3 up(0,1,0);
   ngl::NGLInit *Init = ngl::NGLInit::instance();
   #ifdef WIN32
   	glewInit();
   #endif
   Init->initGlew();
   m_cam= new ngl::Camera(from,to,up,ngl::PERSPECTIVE);
   // now to load the shader and set the values
   // grab an instance of shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    createConstantShader(shader);
    createPhongShader(shader);




    shader->setShaderParam1i("Normalize",1);

    // now pass the modelView and projection values to the shader
    // the shader will use the currently active material and light0 so set them
    m_light = new ngl::Light(ngl::Vec3(150,50,50),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
    // now create our light this is done after the camera so we can pass the
    // transpose of the projection matrix to the light to do correct eye space
    // transformations
    ngl::Mat4 iv=m_cam->getViewMatrix();
    iv.transpose();
    m_light->setTransform(iv);
    m_light->setAttenuation(1,0,0);
    m_light->enable();
    // load these values to the shader as well
    m_light->loadToShader("light");
    shader->createShaderProgram("Colour");
     shader->attachShader("ColourVertex",ngl::VERTEX);
     shader->attachShader("ColourFragment",ngl::FRAGMENT);
     shader->loadShaderSource("ColourVertex","shaders/Colour.vs");
     shader->loadShaderSource("ColourFragment","shaders/Colour.fs");

     shader->compileShader("ColourVertex");
     shader->compileShader("ColourFragment");
     shader->attachShaderToProgram("Colour","ColourVertex");
     shader->attachShaderToProgram("Colour","ColourFragment");

     shader->bindAttribute("Colour",0,"inVert");

     shader->linkProgramObject("Colour");
     (*shader)["Colour"]->use();
     shader->setShaderParam4f("Colour",0,.2,.4,1);
     glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

   // first we create a mesh from an obj passing in the obj file and texture
     m_mesh = new ngl::Obj("models/boid.obj");
//   // now we need to create this as a VAO so we can draw it
     m_mesh->createVAO();
     m_mesh->calcBoundingSphere();
  // create our Bounding Box, needs to be done once we have a gl context as we create VAO for drawing
     m_grid = new DivideSpace(m_centre,m_dimensions);
     m_grid->createBBox();
}


//----------------------------------------------------------------------------------------------------------------------


//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void World::resizeGL(
                        int _w,
                        int _h
                       )
{
  glViewport(0,0,_w,_h);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(50,(float)_w/_h,0.5,150,ngl::PERSPECTIVE);
 }


void World::loadMatricesToColourShader(
                                           ngl::TransformStack &_tx
                                         )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Colour"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  MV= _tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix() ;
  MVP=MV*m_cam->getProjectionMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);

}


//---------------------------------------------------------------------------------------------------------------------


//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void World::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // build our transform stack
   ngl::Transformation trans;
   // Rotation based on the mouse position for our global
   // transform
   ngl::Mat4 rotX;
   ngl::Mat4 rotY;
   // create the rotation matrices
   rotX.rotateX(m_spinXFace);
   rotY.rotateY(m_spinYFace);
   // multiply the rotations
   ngl::Mat4 final=rotY*rotX;
   // add the translations
   final.m_m[3][0] = m_modelPos.m_x;
   final.m_m[3][1] = m_modelPos.m_y;
   final.m_m[3][2] = m_modelPos.m_z;
   // set this in the TX stack
   trans.setMatrix(final);
   m_transformStack.setGlobal(trans);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Colour"]->use();
  loadMatricesToColourShader(m_transformStack);
  if (m_drawBBox)
  {
    m_grid->drawBBox();
  }
  shader->use(m_shaderName);
  m_grid->registerBoidGrid(*m_flock);
  if(m_flock->getObsStatus())
  {
      //scroll through the bounding box list and update the flock
      //move the obstacle recursion algrithm
      //to the flock class in the future
    for(size_t i=0; i < m_obstacleList.size(); ++i)
    {
      m_flock->setCurrentObstacle(&m_obstacleList[i],m_obstacleCount);
      m_obstacleList[i].draw(m_shaderName,m_transformStack,m_cam);
      m_flock->updateFlock(*m_mesh,m_shaderName,m_transformStack,m_cam);
    }

  }
    else
  {
    m_flock->updateFlock(*m_mesh,m_shaderName,m_transformStack,m_cam);
  }
}


//////------------------------------------------------------------------------------------------------------------


void World::createConstantShader(ngl::ShaderLib *constantShader)
{
  m_shaderName = "Constant";
   // we are creating a shader called Constant
   constantShader->createShaderProgram(m_shaderName);
   // now we are going to create empty shaders for Frag and Vert
   constantShader->attachShader("ConstantVertex",ngl::VERTEX);
   constantShader->attachShader("ConstantFragment",ngl::FRAGMENT);
   // attach the source
   constantShader->loadShaderSource("ConstantVertex","shaders/Constant.vs");
   constantShader->loadShaderSource("ConstantFragment","shaders/Constant.fs");
   // compile the shaders
   constantShader->compileShader("ConstantVertex");
   constantShader->compileShader("ConstantFragment");
   // add them to the program
   constantShader->attachShaderToProgram(m_shaderName,"ConstantVertex");
   constantShader->attachShaderToProgram(m_shaderName,"ConstantFragment");
//    now bind the shader attributes for most NGL primitives we use the following
       // now bind the shader attributes for most NGL primitives we use the following
       // layout attribute 0 is the vertex data (x,y,z)
       constantShader->bindAttribute(m_shaderName,0,"inVert");
       // attribute 1 is the UV data u,v (if present)
       constantShader->bindAttribute(m_shaderName,1,"inUV");
       // attribute 2 are the normals x,y,z
       constantShader->bindAttribute(m_shaderName,2,"inNormal");
//    now we have associated this data we can link the shader
   constantShader->linkProgramObject(m_shaderName);
   // and make it active ready to load values
   (*constantShader)[m_shaderName]->use();
}

 //----------------------------------------------------------------------------------------------------


 void World::createPhongShader(ngl::ShaderLib *phongShader)
 {
       m_shaderName = "Phong";

       // we are creating a shader called Phong
       phongShader->createShaderProgram("Phong");
       // now we are going to create empty shaders for Frag and Vert
       phongShader->attachShader("PhongVertex",ngl::VERTEX);
       phongShader->attachShader("PhongFragment",ngl::FRAGMENT);
       // attach the source
       phongShader->loadShaderSource("PhongVertex","shaders/Phong.vs");
       phongShader->loadShaderSource("PhongFragment","shaders/Phong.fs");
       // compile the shaders
       phongShader->compileShader("PhongVertex");
       phongShader->compileShader("PhongFragment");
       // add them to the program
       phongShader->attachShaderToProgram("Phong","PhongVertex");
       phongShader->attachShaderToProgram("Phong","PhongFragment");
       // now bind the shader attributes for most NGL primitives we use the following
       // layout attribute 0 is the vertex data (x,y,z)
       phongShader->bindAttribute("Phong",0,"inVert");
       // attribute 1 is the UV data u,v (if present)
       phongShader->bindAttribute("Phong",1,"inUV");
       // attribute 2 are the normals x,y,z
       phongShader->bindAttribute("Phong",2,"inNormal");

       // now we have associated this data we can link the shader
       phongShader->linkProgramObject("Phong");
//        and make it active ready to load values
       (*phongShader)["Phong"]->use();
 }

///////------------------------------------------------------------------------------------------------


void World::mouseMoveEvent (
                               QMouseEvent * _event
                              )
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_origX;
    int diffy=_event->y()-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event->x();
    m_origY = _event->y();
    updateGL();

  }
  // right mouse translate code
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(_event->x() - m_origXPos);
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    updateGL();

  }

}

//----------------------------------------------------------------------------------------------------------------------


void World::mousePressEvent (
                                QMouseEvent * _event
                               )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate=true;
  }

}

//----------------------------------------------------------------------------------------------------------------------


void World::mouseReleaseEvent (
                                  QMouseEvent * _event
                                 )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
  // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------


void World::wheelEvent(QWheelEvent *_event)
{

  // check the diff of the wheel position (0 means no change)
  if(_event->delta() > 0)
  {
    m_modelPos.m_z+=ZOOM;
  }
  else if(_event->delta() <0 )
  {
    m_modelPos.m_z-=ZOOM;
  }
  updateGL();
}


void World::timerEvent(
                          QTimerEvent *_event
                         )
{
  if(_event->timerId() == m_boidUpdateTimer)
	{
		if (m_animate !=true)
		{
			return;
		}
        update();
		updateGL();
	}
}

void World::keyPress(QKeyEvent *_event)
{

  switch (_event->key())
  {
		case  Qt::Key_Space : m_animate^=true; break;
    case Qt::Key_S : m_checkBoidBoid^=true; break;
	}
}

void World::createNewObstacles(int _cnt)
{
  //if the new value is greater than the obstacle count
  int _value = _cnt-m_obstacleCount;

  if(_value > 0)
  {
     //switch the obstacles check to on
     m_flock->setObsStatus(1);
     float _obsRad=12.0f;
     float _avoidRad=_obsRad*1.5f;

     ngl::Random *rng=ngl::Random::instance();
     for (int i=0; i<_value;i++)
     {
       m_obstacleList.push_back(ObstacleAvoid(rng->getRandomPoint(20,20,20),_obsRad,_avoidRad));
       m_obstacleCount++;
     }
  }
}


void World::setWorldMaxSpeed(float _value)
{
  m_flock->setFlockMaxSpeed(_value);
}

void World::setCurrentShader(int const _value)
{
  switch(_value)
  {
    case 0:
    m_shaderName = "Phong";
    break;

  case 1:
    m_shaderName = "Constant";
    break;
  }
}


