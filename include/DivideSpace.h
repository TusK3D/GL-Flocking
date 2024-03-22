#ifndef DIVIDESPACE_H
#define DIVIDESPACE_H

#include <ngl/Vec3.h>
#include <ngl/BBox.h>
#include "include/Flocking.h"
#include "Boid.h"

/// @file DivideSpace.h
/// @brief brief a spatial partitioning class to manage world collisions
/// @author Tushar Kewlani
/// @version 1.0
/// @date 24/03/13
/// Revision History :
/// Initial Version 24/03/13 (Binary day ;-0 )
/// @class DivideSpace
/// @brief creates the bin-lattice subdivision and collision checks

class DivideSpace
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for DivideSpace
  /// @param [in] _centre the centre of the BBOX container
  /// @param [in] _dimensions the dimensions for the BBCX container
  //----------------------------------------------------------------------------------------------------------------------
  DivideSpace(ngl::Vec3 &_centre,ngl::Vec3 &_dimensions);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief default constructor
  //----------------------------------------------------------------------------------------------------------------------
  DivideSpace(){}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to create the BBox container
  //----------------------------------------------------------------------------------------------------------------------
  void createBBox();
  /// @brief draw method for the container
  inline void drawBBox(){m_bbox->draw();}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to register each boid in its xyz position in the cell
  /// @param [in] _mFlock the flock for which the boids need to be registered
  //----------------------------------------------------------------------------------------------------------------------
  void registerBoidGrid(Flocking &_mFlock);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to return the centre of the BBOX container
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 const getCentre(){return m_centre;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the multiplication factor for repulsion
  //----------------------------------------------------------------------------------------------------------------------
  inline void setRepulsion(float const _value){m_repulsion=_value;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to set the multiplication factor for repulsion
  //----------------------------------------------------------------------------------------------------------------------
  inline void setContainerSizex(float const _x){m_dimensionsX=_x;}

  inline void setContainerSizey(float const _y){m_dimensionsY=_y;}

  inline void setContainerSizez(float const _z){m_dimensionsZ=_z;}


private :
  /// @brief the centre of the BBOX container
  ngl::Vec3 m_centre;
  /// @brief store the x y z dimensions of the BBOX container
  int m_dimensionsX,m_dimensionsY,m_dimensionsZ;
  /// @brief object to create the BBOX container
  ngl::BBox *m_bbox;
  /// @brief the No of rows the space has been divided into
  int m_rows;
  /// @brief the No of columns the space has been divided into
  int m_cols;
  /// @brief the No of depth boxes the space has been divided into
  int m_depth;
  /// @brief the factor which determines the size of the grids in the lattice
  int m_div;
  /// @brief pointer to the current boid being accessed and registered
  Boid *m_curBoid;
  /// @brief the number of boxes either side of the centre of the BBox
  GLfloat gridOffsetX,gridOffsetY,gridOffsetZ;
  /// @brief  multiplication factor for repulsion based on maxSpeed
  float m_repulsion;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief create a force to push the Boids aways from the Bounding Box
  //----------------------------------------------------------------------------------------------------------------------
  void BBoxAvoid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief bounce back gone too far
  //----------------------------------------------------------------------------------------------------------------------
  void BBoxCollision();
  //------------------------------------------------------------------------------------------------------------
};

#endif // CONTAINER_H
