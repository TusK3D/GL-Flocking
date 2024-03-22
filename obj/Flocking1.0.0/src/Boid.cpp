#include "include/Boid.h"
#include <ngl/VAOPrimitives.h>
#include <cmath>
#include <math.h>


Boid::Boid(
                GLint _ID, ngl::Vec3 _pos,
                ngl::Vec3 _dir, float _rad,
                ngl::Vec3 _velocity, bool _prey,
                ngl::Vec3 _accel, float _PredatorMaxSpeed,
                int _boidShape)
{
  // set values from params
  m_boidID=_ID;
  m_pos=_pos;
  m_dir=_dir;
  m_velocity=_velocity;
  m_radius=_rad;
  m_hit=false;
  m_accel=_accel;
  m_prey=_prey;
  m_predatorMaxSpeed = _PredatorMaxSpeed;
  m_contact = 0;
  m_shapeType = _boidShape;

  //if its a predator make it bigger
  if(_prey == 0)
  {
    std::cout<<"predator id added = "<<m_boidID<<std::endl;
    m_radius = _rad*3;
  }
}


//----------------------------------------------------------------------------------------------------------------------

void Boid::loadMatricesToShader(
                                     ngl::TransformStack &_tx,
                                     ngl::Camera *_cam,
                                     std::string const &_shaderName

                                   ) const
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;

  //separate parameters based on shader
  if(_shaderName == "Constant")
  {
    MVP=_tx.getCurrAndGlobal().getMatrix()*_cam->getVPMatrix() ;
  }
  else
  {
    MV=_tx.getCurrAndGlobal().getMatrix()*_cam->getViewMatrix() ;
    MVP=MV*_cam->getProjectionMatrix();
    M=_tx.getCurrentTransform().getMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMat4("M",M);
  }
  shader->setShaderParamFromMat4("MVP",MVP);
}


//----------------------------------------------------------------------------------------------------------------------

void Boid::draw(
                  const ngl::Obj &_mesh,
                   const std::string &_shaderName,
                   ngl::TransformStack &_transformStack,
                   ngl::Camera *_cam
                 )const
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use(_shaderName);
  ngl::Material m;

  // set the material based on predator or prey
  _transformStack.pushTransform();
  {

    _transformStack.setPosition(m_pos);
    _transformStack.setScale(m_radius,m_radius,m_radius);
    _transformStack.setRotation(m_dir.m_x,m_dir.m_y,0);

    loadMatricesToShader(_transformStack,_cam,_shaderName);

    // grab an instance of the primitives for drawing
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    //create materials based on the role
    if(m_prey == 1)
    {
      m.set(ngl::PEWTER );

      if (_shaderName == "Phong")
      {
        m.loadToShader("material");
      }
      else
      {
        shader->setShaderParam4f("Colour",0,0.8,.6,1.0);

      }

      //draw prey
      switch (m_shapeType)
      {
      case 0:
        _mesh.draw();
        break;

      case 1:
        prim->createSphere("sphere",2.0,2.0);
        prim->draw("sphere");

        break;

      case 2:
        // grab an instance of the primitives for drawing
        prim->createCone("cone",2.0,4.0,2,1);
        prim->draw("cone");
        break;
      }
    }

    // draw predators
    else
      {
        m.set(ngl::BRONZE);
        if (_shaderName == "Phong")
        {
          m.loadToShader("material");
        }
        else
        {
          shader->setShaderParam4f("Colour",0.5,0.1,0.1,1.0);

        }

        //switch based on user input

        switch (m_shapeType)
        {
        case 0:
          _mesh.draw();
          break;

        case 1:
          prim->createSphere("sphere",2.0,2.0);
          prim->draw("sphere");

          break;

        case 2:
          // grab an instance of the primitives for drawing
          prim->createCone("cone",2.0,4.0,2,1);
          prim->draw("cone");
          break;
        }

      }

  } // and before a pop
   _transformStack.popTransform();

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

}


//----------------------------------------------------------------------------------------------------------------------

void Boid::move(float _maxSpeed)
{
  // store the last position
  m_lastPos=m_pos;
  if(m_hit)
  {
    m_accel = (m_dir*10)*_maxSpeed;
  }
  else
  {
    //update the velocity vector to accelerate in a particular direction
    m_velocity+=m_accel+(m_wallAvoidFactor*m_wallAvoidMultiplier);
    float _curSpeedsq = m_velocity.length();

    //if a predator add a bit of velocity around prey
    if(m_prey == 0)
    {
      float _maxSpeed = _maxSpeed*m_predatorMaxSpeed;
    }

    //compare current speed with maxspeed factor
    if (_curSpeedsq>_maxSpeed)
    {
      m_velocity.normalize();
      m_velocity*=_maxSpeed;
    }
}

  //update the position of the Boid
  m_pos+=m_velocity;
  // get the next position
  m_nextPos=m_pos+m_velocity;
  m_hit=false;
  //reset the acceleration to zero
  m_accel = 0.0;
}


//----------------------------------------------------------------------------------------------------------------------

void Boid::rotate()
{
    ngl::Vec3 T;
    ngl::Vec3 T_norm;
    ngl::Vec3 L;
    ngl::Vec3 L_norm;
    /// The following section is from :-
    /// Christopher John Kline (1996). [Accessed 2012].
    /// Available from: <http://www.behaviorworks.com/people/ckline/boids>.
    //Calculate Pitch
    m_dir.m_x = -atan(m_velocity.m_y /
      sqrt(m_velocity.m_z*m_velocity.m_z
           + m_velocity.m_x*m_velocity.m_x)) * (180.0 / ngl::PI);
    //calculate yaw
    m_dir.m_y = atan2(m_velocity.m_x, m_velocity.m_z) * (180.0 / ngl::PI);
    ///end of citation

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
