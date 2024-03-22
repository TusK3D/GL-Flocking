#ifndef OBSTACLEAVOID_H
#define OBSTACLEAVOID_H
#include <ngl/VAOPrimitives.h>
#include "Boid.h"

/// @file ObstacleAvoid.h
/// @brief brief a obstacle class to create and draw the obstacles and
/// @author Tushar Kewlani
/// @version 1.0
/// @date 24/03/13
/// Revision History :
/// Initial Version 24/03/13 (Binary day ;-0 )
/// @class ObstacleAvoid
/// @brief Creates the obstacles that live in the world
class ObstacleAvoid
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] default constructor
  //----------------------------------------------------------------------------------------------------------------------
//  ObstacleAvoid(){}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] constructor for Obstacles
  /// @param[in] _pos the poition where the obstacle should be placed
  /// @param[in] _obsRad the radius of the geometry drawn
  /// @param[in] _avoidRad the influence inside which the Boids get pushed away
  //----------------------------------------------------------------------------------------------------------------------
  ObstacleAvoid(ngl::Vec3 _pos, float _obsRad, float _avoidRad);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] get the radius of the obstacle
  //----------------------------------------------------------------------------------------------------------------------
  float getobsRad(){return m_obsRad;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] set the radius of the obstacle
  //----------------------------------------------------------------------------------------------------------------------
  void setobsRad(float _obsRad){m_obsRad = _obsRad;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] get the influence avoid radius for the obstacle
  //----------------------------------------------------------------------------------------------------------------------
  float getavoidRad(){return m_avoidRad;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief []set the influence avoid radius for the object
  //----------------------------------------------------------------------------------------------------------------------
  void setavoidRad(float const _avoidRadius){m_avoidRad=_avoidRadius;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] draw the obstacle in the opengl context
  /// @param[in] _mesh the actual geometry to be drawn
  /// @param[in] the name of the currently active shader
  /// @param[in] transform stack to draw the obstacle
  //----------------------------------------------------------------------------------------------------------------------
  void draw(  const std::string &_shaderName,
              ngl::TransformStack &_transformStack, ngl::Camera *_cam
            ) const ;

  void loadMatricesToShader(
                             ngl::TransformStack &_tx,
                             ngl::Camera *_cam,
                             const std::string &_shaderName
                           )const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] calculate the repulsion force exerted by the obstacle on the flock
  /// @param[in] _focusBoid the boid for which the force is to be calculated
  /// @param[in] _maxSpeed the speed multiplier to limit the speed of the boid
  /// @param[in] _contact check to see if the Boid has collided with the original mesh
  /// @param[in] _obsDist update the distancee betwee the boid and the obstacle if its within the radius
  /// @returns  the force to avoid the obstacle with
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 obstacleForce(Boid const &_focusBoid,float const _maxSpeed, bool &_contact, float &_obsdist);

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief store the radius of the obstace
  //----------------------------------------------------------------------------------------------------------------------
  float m_obsRad;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief store the bounding sphere radius for avoidance range
  //----------------------------------------------------------------------------------------------------------------------
  float m_avoidRad;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the position of the obstacle
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_pos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief check if any boids are in range
  //----------------------------------------------------------------------------------------------------------------------
  bool m_inRange;
  //----------------------------------------------------------------------------------------------------------------------


};

#endif // OBSTACLEAVOID_H
