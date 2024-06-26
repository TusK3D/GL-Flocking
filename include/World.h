#ifndef WORLD_H
#define WORLD_H

#include "include/Flocking.h"
#include "include/DivideSpace.h"
#include "ObstacleAvoid.h"
#include "Parser.h"
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>

// must be included after our stuff becuase GLEW needs to be first
#include <QtOpenGL>

/// @file World.h
/// @brief brief a world class that manages the creation and interaction of the other classes.
/// @author Tushar Kewlani
/// @version 1.0
/// @date 24/03/13
/// Revision History :
/// Initial Version 24/03/13 (Binary day ;-0 )
/// @class World
/// @brief creates and manages all components required to manage the world

class World : public QGLWidget
{
Q_OBJECT        // must include this if you use Qt signals/slots
public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for World
  /// @param [in] _numBoids the number of Boids to create passed from
  /// main window
  /// @param [in] argc the arguments entered
  /// @param [in] the string passed as the arguments
  /// @param [in] parent the parent window to create the GL context in
  //----------------------------------------------------------------------------------------------------------------------
  World(
           int _numBoids,int argc,
           char *argv[],QWidget *_parent
          );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief default constructor
  //----------------------------------------------------------------------------------------------------------------------
  World(){}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief default destructor
  //----------------------------------------------------------------------------------------------------------------------
  ~World();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief toggle the Animation of the lights called from main window
  //----------------------------------------------------------------------------------------------------------------------
  inline void toggleAnimation(){m_animate^=true;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method called from main window for keyPress
  //----------------------------------------------------------------------------------------------------------------------
  void keyPress(
                 QKeyEvent *_event
               );
  //-------------------------------------------------------------------------------------------------
  /// @brief return the currently active flock
  //-------------------------------------------------------------------------------------------------
  inline Flocking *getCurrentFlock(){return m_flock;}
  //-------------------------------------------------------------------------------------------------
  /// @brief return the currently active grid
  //-------------------------------------------------------------------------------------------------
  inline DivideSpace *getCurrentGrid() const{return m_grid;}
  //-------------------------------------------------------------------------------------------------
  /// @brief return the obstacle count
  //-------------------------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------------------------
  /// @brief return the indexed active obstacle
  //-------------------------------------------------------------------------------------------------
  inline ObstacleAvoid getCurrentObstacle(int _index) const{return m_obstacleList[_index];}
  //-------------------------------------------------------------------------------------------------
  inline int getObstacleCount(){return m_obstacleCount;}

  inline void setCurrentFLock(Flocking _flock){m_flock = &_flock;}

  /// @brief set the max speed for the Flock
  void setWorldMaxSpeed(float _value);

  /// @brief create obstacles
  void createNewObstacles(int _cnt);

  /// @brief toggle if BBOX should be drawn
  inline void toggleBBoxDraw(bool _value){m_drawBBox = _value;}

  /// @brief toggle the current shader to be drawn
  void setCurrentShader (const int _value);


  /// @brief create the mesh for the Boid




private :

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the x rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_spinXFace;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to store the y rotation mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_spinYFace;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if the mouse button is pressed when dragging
  //----------------------------------------------------------------------------------------------------------------------
  bool m_rotate;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if the Right mouse button is pressed when dragging
  //----------------------------------------------------------------------------------------------------------------------
  bool m_translate;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous x mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_origX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous y mouse value
  //----------------------------------------------------------------------------------------------------------------------
  int m_origY;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous x mouse value for Position changes
  //----------------------------------------------------------------------------------------------------------------------
  int m_origXPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the previous y mouse value for Position changes
  //----------------------------------------------------------------------------------------------------------------------
  int m_origYPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the model position for mouse movement
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_modelPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Our Camera
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Camera *m_cam;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief timer to change the sphere position by calling update()
  //----------------------------------------------------------------------------------------------------------------------
  int m_boidUpdateTimer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our transform stack for drawing the teapots
  //----------------------------------------------------------------------------------------------------------------------
  ngl::TransformStack m_transformStack;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if animation is active or not
  //----------------------------------------------------------------------------------------------------------------------
  bool m_animate;
  /// @brief our model
  ngl::Obj *m_mesh;
  //----------------------------------------------------------------------------------------------------------------------
  ///@brief create  a constant shader
  //----------------------------------------------------------------------------------------------------------------------
  void createConstantShader(ngl::ShaderLib *constantShader);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief create a phong shader
  //----------------------------------------------------------------------------------------------------------------------
  void createPhongShader(ngl::ShaderLib *phongShader);
  //----------------------------------------------------------------------------------------------------------------------



protected:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  //----------------------------------------------------------------------------------------------------------------------
   void initializeGL();
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  //----------------------------------------------------------------------------------------------------------------------
  void resizeGL(
                const int _w,
                const int _h
               );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is the main gl drawing routine which is called whenever the window needs to
  /// be re-drawn
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL();

private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a dynamic pointer to contain our flock
  //----------------------------------------------------------------------------------------------------------------------
  Flocking *m_flock;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a array to contain our obstacles
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ObstacleAvoid> m_obstacleList;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the bounding box to contain the flock to be changed to container type
  //----------------------------------------------------------------------------------------------------------------------
  DivideSpace *m_grid;
  //----------------------------------------------------------------------------------------------------------------------
  ///@brief the centre and dimensions of the grid
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_centre;
  ngl::Vec3 m_dimensions;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief flag to indicate if we need to do boidboid checks
  //----------------------------------------------------------------------------------------------------------------------
  bool m_checkBoidBoid;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief light for the scene
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Light *m_light;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief save the shader name for the scene
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_shaderName;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief obstacle count for the scene
  //----------------------------------------------------------------------------------------------------------------------
  int m_obstacleCount;
  //----------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief check to draw BBox
  //----------------------------------------------------------------------------------------------------------------------
  bool m_drawBBox;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------

  void mouseMoveEvent (
                       QMouseEvent * _event
                      );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
   void mousePressEvent (
                        QMouseEvent *_event
                       );

   //----------------------------------------------------------------------------------------------------------------------
   /// @brief this method is called everytime the mouse button is released
   /// inherited from QObject and overridden here.
   /// @param _event the Qt Event structure
   //----------------------------------------------------------------------------------------------------------------------
   void mouseReleaseEvent (
                            QMouseEvent *_event
                           );
   //----------------------------------------------------------------------------------------------------------------------
   /// @brief this method is called everytime the mouse wheel is moved
   /// inherited from QObject and overridden here.
   /// @param _event the Qt Event structure
   //----------------------------------------------------------------------------------------------------------------------
   void wheelEvent(
                     QWheelEvent *_event
                  );

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the timer event triggered from the timers
  /// @param _even the event of the timer triggered by Qt
  //----------------------------------------------------------------------------------------------------------------------
  void timerEvent(
                  QTimerEvent *_event
                 );
  //----------------------------------------------------------------------------------------------------------------------

  /// @brief reset the sphere array
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief create an instance of a constant shader
  ngl::ShaderLib *constantShader;

  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader(
                         ngl::TransformStack &_tx
                       );
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToColourShader(
                                   ngl::TransformStack &_tx
                                 );
  //----------------------------------------------------------------------------------------------------------------------
};

#endif
