#include "include/Flocking.h"
#include <boost/foreach.hpp>


const static int s_extents=30;

void Flocking::createBoidFlock(GLint _boidCount, Parser const &_parse)
{
  m_parse = _parse;
  m_boidArray.erase(m_boidArray.begin(),m_boidArray.end());
  ngl::Vec3 _dir;
  // loop and create the initial particle list
  for(GLint i=0; i<_boidCount; ++i)
  {
    ngl::Vec3 _vel=(rng->getRandomVec3()*.5)*m_parse.getVelocity();
    // add the boids to the end of the particle list
    m_boidArray.push_back(Boid(i,rng->getRandomPoint(s_extents,s_extents,s_extents),_dir,
                                m_parse.getRadius(),_vel,m_parse.getPreyStatus(),m_parse.getAccel(),
                               m_parse.getpredatorMaxSpeedFactor(),m_parse.getBoidShape()));

   }
  m_maxspeed=m_parse.getMaxSpeed();
  m_maxaccel= m_maxspeed*.1;
  m_sepFactor = m_parse.getSepFactor();
  m_cohesionFactor = m_parse.getCohesionFactor();
  m_alignFactor = m_parse.getAlignFactor();
  m_staticPredatorCount = 0;
  m_myPrey.m_predatorCount = 0;
  m_active = 0;
  m_userSearchRegion = 8.0;
  m_userSearchPercent = 0.2f;
  m_flockWallAvoidMultiplier = 2.0f;
  m_userObsAvoid = 1.0f;
  m_separationDist = 8.0f;
  m_neighbourSeekDist = 20.0f;
  m_userPredatorSeekFactor=1.0f;
  m_userPreyFleeFactor = 1.0f;
}


//----------------------------------------------------------------------------------------------------------------------

void Flocking::addPredatorBoid()
{
  ngl::Vec3 pos(4,4,4);
  ngl::Vec3 _dir(0,0,0);
  ngl::Vec3 _vel(2,2,2);
  int cnt = m_boidCount;
  //create the predator boids with prey parameter set to 0 or false
  m_boidArray.push_back(Boid(cnt,rng->getRandomPoint(s_extents,s_extents,s_extents),_dir,
                              m_parse.getRadius(),_vel,0,m_parse.getAccel(),
                                 m_parse.getpredatorMaxSpeedFactor(),m_parse.getBoidShape()));
  ++m_myPrey.m_predatorCount;
  ++m_boidCount;



}


//----------------------------------------------------------------------------------------------------------------------

void Flocking::addBoidFlock(int _cnt)
{
  for (int i=0; i<_cnt;++i)
  {
      ngl::Vec3 _dir(0,0,0);
      ngl::Vec3 _vel(4,4,4);
      m_boidArray.push_back(Boid(m_boidCount,rng->getRandomPoint(s_extents,s_extents,s_extents),_dir,
                                  m_parse.getRadius(),_vel,1,m_parse.getAccel(),
                                      m_parse.getpredatorMaxSpeedFactor(),m_parse.getBoidShape()));
      ++m_boidCount;
  }
}


//----------------------------------------------------------------------------------------------------------------------

void Flocking::deleteBoids()
{
    std::vector<Boid>::iterator end=m_boidArray.end();
  if(--m_boidCount==0)
  {
    m_boidCount=1;
  }
  else
  {
    m_boidArray.erase(end-1,end);
  }
}


//------------------------------------------------------------------------------------------------

void Flocking::editFlockCount(int const &_value)
{
  int _difference = m_boidCount - _value;
  if (_difference > 0 )
  {
    for(int i=_difference; i>0; --i)
    {
      deleteBoids();
    }
  }
  else
    if (_difference < 0)
    {
      _difference*=-1;
      addBoidFlock(_difference);
    }
}


//------------------------------------------------------------------------------------------------

void Flocking::updateFlock(const ngl::Obj &_mesh, const std::string &_shaderName,ngl::TransformStack &_transformStack,
                           ngl::Camera *_cam)
{
  BOOST_FOREACH(Boid &b, m_boidArray)
  {
    m_behaviour = 0;
    //calculate the behaviour for the current
    m_behaviour=behaviour(b);
    //if the force calculated is greater than the maximum acceleration
    if(m_maxaccel < m_behaviour.length())
    {
      b.setAccel(m_behaviour*m_maxaccel);
    }
    else
    {
      b.setAccel(m_behaviour);
    }
    b.move(m_maxspeed);
    b.rotate();
    b.draw(_mesh,_shaderName,_transformStack,_cam);
  }
}


//--------------------------------------------------------------------------------------------

ngl::Vec3 Flocking::behaviour(Boid &_focusBoid)
{
    ngl::Vec3 _force=0;
    ngl::Vec3 _sum=0;
    ngl::Vec3 _separation=0;
    ngl::Vec3 _alignment=0;
    ngl::Vec3 _velSum=0;
    ngl::Vec3 _cohesion=0;
    ngl::Vec3 _posSum=0;
    ngl::Vec3 _desiredLoc=0;
    ngl::Vec3 _preyPosSum=0;
    ngl::Vec3 _preyVelSum=0;
    ngl::Vec3 _predatorForce;
    int counter=0;
    float _preythreshDist=15.0f;
    float _preyDist=0.0f;
    ngl::Vec3 _preyDiff;
    int _preyCounter=0;
    int _predatorCounter=0;
    ngl::Vec3 _preyForceSum=0;
    ngl::Vec3 _preyForce=0;
    ngl::Vec3 _predatorSeek=0;
    Boid *_predatorBoid = new Boid;
    ngl::Vec3 _predAvoidDist = 0;
    int _predRepCounter = 0;


    _focusBoid.setBoidShape(m_boidShape);
    _focusBoid.setBoidWallAvoidMultiplier(m_flockWallAvoidMultiplier);

    /*
    /// Modified from :-
    /// Daniel Shiffman (2012). Nature of code Chapter 6.Autonomous Agents [online].
    /// [Accessed 2012]. Available from: <http://natureofcode.com/book/chapter-6-autonomous-agents/>.
    */

    _force=0;
    int x,y,z=0;
    _focusBoid.getGridAddress(x,y,z);
    GLint _neighbourCount = 0;
    _neighbourCount = m_myNeighbours[x][y][z].m_neighbourCount;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(_neighbourCount != 0)
    {
      for(int i=0;i<_neighbourCount && i < (m_boidCount*m_userSearchPercent);i++)
      {

        Boid *_neighbourBoid = m_myNeighbours[x][y][z].m_inCurrentBlock[i];
        ngl::Vec3 _dist = _focusBoid.getPos() - _neighbourBoid->getPos();
        GLfloat diff = _dist.length();
        if((_neighbourBoid->getBoidID()) != _focusBoid.getBoidID())
        {
          if (diff < m_userSearchRegion)
          {
             if(_focusBoid.getPrey() == 1)
            {

            float radiussq = (_focusBoid.getRadius()*m_separationDist);
            if(diff < m_neighbourSeekDist)
            {
              //alignement
              _velSum+= _neighbourBoid->getVel();
              //cohesion
              _posSum+=_neighbourBoid->getPos();
            }
              //separation
              if(diff<radiussq)
              {
                ngl::Vec3 _distNorm = _dist;
                _distNorm.normalize();
                _distNorm = _distNorm/diff;
                _sum+=_distNorm;
              }
              ///////end of citation

            counter++;
            }
          }
          if(_focusBoid.getPrey() == 0 && diff < _preythreshDist)
          {

              _preyPosSum+=_neighbourBoid->getPos();
              _preyVelSum+=_neighbourBoid->getVel();
              _predatorCounter++;
          }

        }
        else
          ++i;
      }

////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////calculate prey forces
      //check to see if there are any predators in the scene
        for(size_t i= 0; i< m_myPrey.m_mycurrentPredator.size(); i++)
           {
             _predatorBoid = m_myPrey.m_mycurrentPredator[i];
             _preyDiff = _focusBoid.getPos()-_predatorBoid->getPos();
             _preyDist = _preyDiff.length();
            //if the distance between the prey and the predator is below a threshold
             //if the Boid is not looking at itself
             if (_preyDist < _preythreshDist && _preyDist != 0)
             {
               ngl::Vec3 _preyDistNorm = _preyDiff;
               _preyDistNorm.normalize();
               _preyDistNorm = _preyDistNorm/_preyDist;
               _preyForceSum+=_preyDistNorm;
               _preyCounter++;
            //if the active boid is a predator calculate separation from other predators
             if (_focusBoid.getPrey() == 0)
             {
               _predAvoidDist+=_preyDistNorm;
               _predRepCounter++;
             }

            }

          }



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // calculate the behaviour forces
      if(counter !=0)
      {
         //average out the alignement parameter
          if(_velSum.length() != 0)
          {
            _velSum/=counter;
            _velSum.normalize();
            _velSum*=m_maxspeed;
            _alignment=_velSum-_focusBoid.getVel();
          }

            //average out the cohesion parameter
            _posSum=_posSum/counter;
            _desiredLoc=_posSum-_focusBoid.getPos();
            _desiredLoc.normalize();
            _desiredLoc=_desiredLoc*m_maxspeed;
            _cohesion = _desiredLoc-_focusBoid.getVel();

            //average out the separation parameter
            _sum=_sum/counter;
            if(_sum.length() != 0)
            {
            _sum.normalize();
            _sum*=m_maxspeed;
            _separation = _sum - _focusBoid.getVel();
            }

            /////////calculate predator forces
            if(_preyCounter !=0)
            {
              _preyForceSum=_preyForceSum/_preyCounter;
              if(_preyForceSum.length() != 0)
              {
                _preyForceSum.normalize();
                _preyForceSum*=m_maxspeed;
                _preyForce=_preyForceSum-_focusBoid.getVel();
              }
            }

      }


      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //calcualte predator prey relationship if the predator was within range
      if(_predatorCounter != 0)
        {
            if(_preyVelSum !=0)
            {
              ngl::Vec3 _predatorSeekNorm;
              _preyPosSum = _preyPosSum/_predatorCounter;
              _predatorSeek = _preyPosSum - _focusBoid.getPos();
              _predatorSeekNorm = _predatorSeek;
              _predatorSeekNorm.normalize();
              _predatorSeek=(_predatorSeek*(m_maxspeed*_focusBoid.getPredatorMaxSpeed())/
                                                                  _predatorSeek.length());
              _predatorForce=_predatorSeek-_focusBoid.getVel();
            }

            //apply separation between predators
            if (_predRepCounter)
            {
              _predAvoidDist = _predAvoidDist/_predRepCounter;
              if (_predAvoidDist.length() != 0)
              {
                _predAvoidDist.normalize();
                _predAvoidDist*=m_maxspeed;
                _predAvoidDist=_predAvoidDist-_focusBoid.getVel();
              }
            }
        }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //     Boids tend to a spot in the Grid
            ngl::Vec3 location;
            rng->setSeed();
            location = rng->getRandomPoint(50,50,50);
            ngl::Vec3 tendToLocation;
            tendToLocation = moveToLoc(_focusBoid,location);

    //    Apply all the flocking forces calculated
          _force = ((_alignment*(m_alignFactor*1.5f))+(_cohesion*m_cohesionFactor*0.5)
                                +(_separation*(m_sepFactor*2.0f))+tendToLocation*(.01f))*1.0;
     // apply the predator prey behaviour
          _force+=((_predatorForce*m_userPredatorSeekFactor)+
                   (_predAvoidDist*0.5)*1.0f)+(_preyForce*m_userPreyFleeFactor*2.0f)*1.0f;

   }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //apply obstacle avoid forces
    if(m_active == 1)
    {
      bool _contact = _focusBoid.getContact();
      float _obsDist = 0.0;
      obsAvoidForces =m_obstacle->obstacleForce(_focusBoid,m_maxspeed, _contact,_obsDist);
      if(_contact == 1)
      {
        //reduce the behaviour forces by a factor of the distance from the obstacle
        _force = _force/(1/(_obsDist*200));
        //reverse the direction of the boids
        _focusBoid.reverse();
      }
      else
      {
        if(_obsDist != 0)
        {
          //reduce the behaviour forces by a factor of the distance from the obstacle
          _force = _force/(1/(_obsDist*.0002));
          _force+=(obsAvoidForces*(-5.0f*m_userObsAvoid));
        }
      }
    }

    return _force;
}


//-----------------------------------------------------------------------------------------------------------

ngl::Vec3 Flocking::moveToLoc(Boid const &_focusBoid, ngl::Vec3 const &_Loc)
{
    //calculate a random vector for the boids to follow
    ngl::Vec3 desiredLoc;
    ngl::Vec3 tendTo;
    desiredLoc = _Loc - _focusBoid.getPos();
    desiredLoc.normalize();
    desiredLoc=desiredLoc*m_maxspeed;
    tendTo = desiredLoc-_focusBoid.getVel();
    return tendTo;
}

//-----------------------------------------------------------------------------------------------------------

void Flocking::setFlockMaxSpeed(double const _value)
{
//set the max speed of the flock
    m_maxspeed = _value*.4;
//clamp the values if they are below 3
  if(_value < 3)
  {
    m_maxaccel = m_maxspeed * 0.1;
  }
  else
  {
    m_maxaccel = (m_maxspeed*.05)+(m_maxspeed/10);
  }
}

//-----------------------------------------------------------------------------------------------------------

void Flocking::setFlockWallAvoidMultiplier(const float _value)
{
  m_flockWallAvoidMultiplier = _value;
}

///////---------------------------------------------------------




