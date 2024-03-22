#ifndef BOID_H
#define BOID_H


#include <ngl/Camera.h>
#include <ngl/ShaderLib.h>
#include <ngl/TransformStack.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Vec3.h>
#include <ngl/Random.h>
#include <ngl/Obj.h>
#include <ngl/Material.h>
#include <cmath>
#include <vector>
#include "Parser.h"

/// @file Boid.h
/// @brief brief a boid class to describe the Boids
/// @author Tushar Kewlani
/// @version 1.0
/// @date 24/03/13
/// Revision History :
/// Initial Version 24/03/13 (Binary day ;-0 )
/// @class Boid
/// @brief Contains all the charactristics that define the Boids


class Boid
{
public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for Boid
  /// @param [in] _pos the position to initalise the boids in
  /// @param [in] _dir the direction to initalise the boids in
  /// @param [in] _rad the radius to initalise the boids to
  /// @param [in] _velocity the velocity to initalise the boids in
  /// @param [in] _prey the nature of the Boid as either a Predator or Prey
  /// @param [in] _accel the acceleration to initalise the boids to
  /// @param [in] _PredatorMaxSpeed the max speed multiplier for the Predator
  /// @param [in] _pos the initial shape of the Boids
  //----------------------------------------------------------------------------------------------------------------------
    Boid(
        GLint _ID, ngl::Vec3 _pos,
        ngl::Vec3 _dir,float _rad,
        ngl::Vec3 _velocity, bool _prey,
        ngl::Vec3 _accel, float _PredatorMaxSpeed,
        int _boidShape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief default constructor
    //----------------------------------------------------------------------------------------------------------------------
    Boid(){;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor
    //----------------------------------------------------------------------------------------------------------------------
   ~Boid(){}
  /// @brief draw method for the Boids
  void draw(
             const ngl::Obj &_mesh,
             const std::string &_shaderName,
             ngl::TransformStack &_transformStack, ngl::Camera *_cam
            )const ;

  void loadMatricesToShader(
                             ngl::TransformStack &_tx,
                             ngl::Camera *_cam,
                             const std::string &_shaderName
                           )const;

  inline void reverse(){m_velocity=m_velocity*-1.0;
                         glRotatef(90,1,0,0);}
	inline void setHit(){m_hit=true;}
	inline void setNotHit(){m_hit=false;}
	inline bool isHit()const {return m_hit;}

  inline ngl::Vec3 getPos() const {return m_pos;}
  inline ngl::Vec3 getVel() const {return m_velocity;}

  inline ngl::Vec3 getNextPos() const {return m_nextPos;}

  inline GLfloat getRadius() const {return m_radius;}

  inline void setDirection(ngl::Vec3 const _d){m_velocity=_d;}

  inline ngl::Vec3 getDirection() const { return m_velocity;}

  inline ngl::Vec3 getaccel() const {return m_accel;}

  inline void setAccel(ngl::Vec3 const _accel){m_accel = _accel;}

  inline GLint getBoidID() const {return m_boidID;}

  inline float getPredatorMaxSpeed() const {return m_predatorMaxSpeed;}

  inline ngl::Vec3 getAvoidFactor() const {return m_wallAvoidFactor;}

  inline void setAvoidFactor(ngl::Vec3 const &_avoidFactor)  {m_wallAvoidFactor = _avoidFactor;}

  inline void setGridAddress(const int &x,const int &y,const int &z){
        m_gridAddress.m_x=x;  m_gridAddress.m_y=y; m_gridAddress.m_z=z;}

  void getGridAddress(int &_gridX,int &_gridY,int &_gridZ)
          {_gridX=m_gridAddress.m_x;_gridY=m_gridAddress.m_y;_gridZ=m_gridAddress.m_z;}

  inline void setPrey(int const _switch){m_prey=_switch;}

  inline int getPrey() const {return m_prey;}

  inline bool getContact() const {return m_contact;}

  inline void setContact (bool const _contact){m_contact = _contact;}

  inline void setBoidShape(int const _value){m_shapeType = _value;}

  inline void setBoidWallAvoidMultiplier(float const _value){m_wallAvoidMultiplier = _value;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief move the Boid based on the behaviour
  /// @param[in] _maxSpeed the max speed at which the current flock can move
  //----------------------------------------------------------------------------------------------------------------------
  void move(float _maxSpeed);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief rotate the Boid and orient it to the direction of travel
  //----------------------------------------------------------------------------------------------------------------------
  void rotate();

protected :
  /// @brief the unique ID for each boid
  GLint m_boidID;
  /// @brief the position of the boid
  ngl::Vec3 m_pos;
  /// @brief the radius of the boid */
  float m_radius;
  /// @brief the velocity of the boid*/
  ngl::Vec3 m_velocity;
  /// @brief flag to indicate if the boid has been hit by ray */
  bool m_hit;
  /// @brief grid address of each boid
  ngl::Vec3 m_gridAddress;
  /// @brief the direction of the boid
  ngl::Vec3 m_dir;
  /// @brief the last position of the boid
  ngl::Vec3 m_lastPos;
  /// @brief the next position of the boid
  ngl::Vec3 m_nextPos;
  /// @brief the acceleration of the boid
  ngl::Vec3 m_accel;
  /// @brief object to access the math functions
//  MyMath *math = new MyMath;
  /// @brief multiplier for the force to push away from the wall
  ngl::Vec3 m_wallAvoidFactor;
  /// @brief whether the boid is a predator or a prey
  bool m_prey;
  /// @brief check if contact with obstacle has been made
  bool m_contact;
  /// @brief store the speed multiplier for the predator
  float m_predatorMaxSpeed;
  /// @brief the shape to be rendered for the Boid
  int m_shapeType;
  /// @brief the wall avoidance Multiplier
  int m_wallAvoidMultiplier;


};


#endif
