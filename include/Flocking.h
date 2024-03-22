#ifndef FLOCKING_H
#define FLOCKING_H
#include "include/Boid.h"
#include "ObstacleAvoid.h"
#include "Parser.h"

/// @file Flocking.h
/// @brief brief a flocking class to describe the behaviour of the flock
/// @author Tushar Kewlani
/// @version 1.0
/// @date 24/03/13
/// Revision History :
/// Initial Version 24/03/13 (Binary day ;-0 )
/// @class Flocking
/// @brief Contains all the charactristics that define the Flock


class Flocking : public Boid
{
  public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default Constructor for Flock
  //----------------------------------------------------------------------------------------------------------------------
  Flocking(){}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief default constructor
  //----------------------------------------------------------------------------------------------------------------------
  ~Flocking(){}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called by the world to update the Boid positions and create the behaviour
  /// @param[in] _mesh the mesh to be rendered for the Boids
  /// @param[in] _shaderName the shader to be used for the Boid
  /// @param[in] _transformStack the transform stack to be used for drawing the Boids
  /// @param[in] _cam the camera to view thw world
  //----------------------------------------------------------------------------------------------------------------------
  void updateFlock(const ngl::Obj &_mesh, const std::string &_shaderName,
                   ngl::TransformStack &_transformStack, ngl::Camera *_cam);

  inline GLint getBoidCount() const {return m_boidCount;}

  inline void setBoidCount(GLint const _numBoids){m_boidCount=_numBoids;}

  inline Boid getIndexBoid(int const _index) const {return m_boidArray[_index];}

  inline Boid* getCurrentBoid(int const _index) {return &m_boidArray[_index];}

  inline GLfloat getSepFactor() const {return m_sepFactor;}

  inline GLfloat getCohesionFactor() const {return m_cohesionFactor;}

  inline void setCohesionFactor(float const &_value){m_cohesionFactor = (_value);}

  inline void setSepararationFactor(float const &_value){m_sepFactor =_value;}

  inline void setAlignFactor(float const &_value){m_alignFactor =_value;}

  inline GLfloat getAlignFactor() const {return m_alignFactor;}

  inline void setUserObsAvoid(float const _value){m_userObsAvoid = _value;}

  inline void setUserSearchRegion(float const _value){m_userSearchRegion = _value;}

  inline float getUserSearchRadius() {return m_userSearchRegion;}

  inline void setNeighbourPercent(int const _value){m_userSearchPercent = (_value*.01f);}



  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to add predator Boids to the scene
  //----------------------------------------------------------------------------------------------------------------------
  void addPredatorBoid();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to move Boids to random location per frame to simulate wander
  /// @param [in] _focusBoid the current Boid to be put to wander
  /// @param [in] _Loc the location the Boid should seek
  /// @returns the force with which to move in that direction
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 moveToLoc(Boid const &_focusBoid, ngl::Vec3 const &_Loc);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief add Boids to the Flock
  /// @param [in] _cnt the No of Boids to be added
  //----------------------------------------------------------------------------------------------------------------------
  void addBoidFlock(int _cnt);
  /// @brief set the current obstacle
  inline void setCurrentObstacle(ObstacleAvoid *_obstacle, int const &_obsCount) {m_obstacle = _obstacle;
                                                                                  m_obstacleCount = _obsCount;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief initialize the flock
  /// @param [in] _boidCount the No of Boids to be created
  /// @param [in] _parse the default values parsed from the file
  //----------------------------------------------------------------------------------------------------------------------
  void createBoidFlock(GLint _boidCount,Parser const &_parse);

  inline bool getObsStatus() const {return m_active;}

  inline void setObsStatus(bool const _active){m_active=_active;}

  inline GLfloat getmaxspeed() const {return m_maxspeed;}

  inline GLfloat getmaxaccel() const {return m_maxaccel;}

  inline void setMaxSpeed(float const &_value){m_maxspeed = _value;}

  inline void setMaxAccel(float const &_value){m_maxaccel = _value;}

  inline void setSeparationDistance(float const &_value){m_separationDist = _value;}

  inline float getSeparationDistance () const{return m_separationDist;}

  inline void setSeekDistance(float const &_value){m_neighbourSeekDist = _value;}

  inline float getSeekDistance () const{return m_neighbourSeekDist;}

  inline void setUserPredSeekFactor(float const &_value){m_userPredatorSeekFactor = _value;}

  inline void setUserPreyfleeFactor(float const &_value){m_userPreyFleeFactor = _value;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief update boid count based on UI input
  /// @param[in] _value the final count of the flock as required
  //----------------------------------------------------------------------------------------------------------------------
  void editFlockCount(int const &_value);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a struct that points to the neighbours for each Boid
  //----------------------------------------------------------------------------------------------------------------------
  struct neighbourList
  {
    int m_neighbourCount;
    // pointers of boid as there will be lots of boids
    std::vector<Boid*> m_inCurrentBlock;
  }m_myNeighbours[20][20][20];
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief specify the predator or prey behaviour
  //----------------------------------------------------------------------------------------------------------------------
  struct preyList
  {
    std::vector<Boid*> m_mycurrentPredator;
    int m_predatorCount;
  }m_myPrey;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief initialize only once
  int m_staticPredatorCount;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the max speed for the flock
  void setFlockMaxSpeed(const double _value);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the shape for the flock
  inline void setFlockShape(int _value){m_boidShape = _value;}
  //----------------------------------------------------------------------------------------------------------------------
  void setFlockWallObstacle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the shape for the flock
  void setFlockWallAvoidMultiplier(float const _value);
  //----------------------------------------------------------------------------------------------------------------------


  private :
  /// @brief create a vector to store boids in
  std::vector<Boid> m_boidArray;
  /// @brief total boids in the list
  GLint m_boidCount;
  /// @brief values to store separation, alignment and cohesion
  ngl::Vec3 m_behaviour;
  /// @brief max speed for the boid
  GLfloat m_maxspeed;
  /// @brief max accel of the boid to remove jerky movement
  GLfloat m_maxaccel;
  /// @brief initialize a random function call to the NGL library
  ngl::Random *rng=ngl::Random::instance();
  /// @brief calculate the steering force
  ngl::Vec3 behaviour(Boid &_focusBoid);
  /// @brief create an object to calculate the obstacle avoid forces
  ObstacleAvoid *m_obstacle;
  /// @brief store obstacle avoid forces
  ngl::Vec3 obsAvoidForces;
  /// @brief if the obstacle is active or not
  bool m_active;
  /// @brief store pointer to configuration object used
  Parser m_parse;
  /// @brief user multipier for the three basic steering behaviours
  GLfloat m_sepFactor;
  GLfloat m_alignFactor;
  GLfloat m_cohesionFactor;
  /// @brief the shape to be used for the Boid
  int m_boidShape;
  //@brief user entered avoid factor for obstacles
  float m_userObsAvoid;
  //@brief user entered region search for neighbours
  float m_userSearchRegion;
  //@brief user entered prey flee factor
  float m_userPreyFleeFactor;
  //@brief user entered predator seek factor
  float m_userPredatorSeekFactor;
  //@brief user entered percentage of neighbours to search for
  float m_userSearchPercent;
  //@brief user entered percentage of neighbours to search for
  float m_flockWallAvoidMultiplier;
  //@brief user entered separation range
  float m_separationDist;
  //@brief user entered alignemnt and cohesion distance
  float m_neighbourSeekDist;
  //@brief Keep track of obstacle count
  float m_obstacleCount;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief delete boids from the vector
  //----------------------------------------------------------------------------------------------------------------------
  void deleteBoids();

};

#endif // FLOCKING_H
