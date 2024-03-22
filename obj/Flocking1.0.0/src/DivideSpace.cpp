#include <cmath>
#include "include/DivideSpace.h"



DivideSpace::DivideSpace(ngl::Vec3 &_centre, ngl::Vec3 &_dimensions)
{
  m_centre = _centre;
  m_dimensionsX = int(_dimensions.m_x);
  m_dimensionsY = int(_dimensions.m_y);
  m_dimensionsZ = int(_dimensions.m_z);
  m_div=20;
  m_repulsion = 1.0f;
}

//----------------------------------------------------------------------------------------------------------------------


void DivideSpace::createBBox()
{
  m_bbox = new ngl::BBox(m_centre,m_dimensionsX,m_dimensionsY,m_dimensionsZ);

  m_bbox->setDrawMode(GL_LINE);
}

//----------------------------------------------------------------------------------------------------------------------


void DivideSpace::registerBoidGrid(Flocking &_mFlock)
{
/*
/// Modified from :-
/// Daniel Shiffman (2012). Nature of code Chapter 6.Autonomous Agents [online].
/// [Accessed 2012]. Available from: <http://natureofcode.com/book/chapter-6-autonomous-agents/>.
*/

//   register the address of each boid
  //calculate the number of cells to fit the boids into
  m_cols=m_dimensionsX/m_div;
  m_rows=m_dimensionsY/m_div;
  m_depth=m_dimensionsZ/m_div;

  //loop through the boids position to fit into the appropriate cells
  for (int l=0;l<m_cols;l++)
    for (int m=0;m<m_rows;m++)
      for (int n=0;n<m_depth;n++)
      {
        _mFlock.m_myNeighbours[l][m][n].m_neighbourCount=0;
        _mFlock.m_myNeighbours[l][m][n].m_inCurrentBlock.clear();
        _mFlock.m_myPrey.m_mycurrentPredator.clear();
      }

  for(int i=0;i<_mFlock.getBoidCount();++i)
  {
    int x,y,z=1;
    //set the offset for the values to be greater than 0
    gridOffsetX = m_cols/2;
    gridOffsetY = m_rows/2;
    gridOffsetZ = m_depth/2;
    m_curBoid = new Boid;
    m_curBoid = _mFlock.getCurrentBoid(i);
    ngl::Vec3 _vb=m_curBoid->getPos();
    //offset the position values based on the grid and truncate them to ints
    x=int((_vb.m_x)/m_div)+gridOffsetX;
    y=int((_vb.m_y)/m_div)+gridOffsetY;
    z=int((_vb.m_z)/m_div)+gridOffsetZ;

    //check to make sure the Boids lie in the Grid
    if((x >= 0) && (x <= m_cols) && (y >= 0) && (y <= m_rows) && (z > 0) && (z <= m_depth))
    {
        _mFlock.m_myNeighbours[x][y][z].m_inCurrentBlock.push_back(m_curBoid);
        _mFlock.m_myNeighbours[x][y][z].m_neighbourCount+=1;
        m_curBoid->setGridAddress(x,y,z);
    }

    ///end of citation

    if (m_curBoid->getPrey()==0)
    {
        // if a predator is detected it is added to the vector
        _mFlock.m_myPrey.m_mycurrentPredator.push_back(m_curBoid);
    }

    //check to see if the Boids are in the corner of the subdivivided grid space
    if ((x == 1 || x == m_cols-1) ||  (y == 1 || y == m_rows-1) ||
                  (z == 1 || z == m_depth-1))
    {
        //calculate a steer o turn them in the reflected direction calulated
        BBoxAvoid();
    }
    else
    {
        //if they are outside the grid
      if (x == m_cols || y == m_rows || z == m_depth)
      {
          //reflect them back
          BBoxCollision();
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------


void DivideSpace::BBoxAvoid()
{
  //create an array of the extents of the bounding box
  float ext[6];
  ext[0]=ext[1]=(m_bbox->height()/2.0f);
  ext[2]=ext[3]=(m_bbox->width()/2.0f);
  ext[4]=ext[5]=(m_bbox->depth()/2.0f);
  // Dot product needs a Vector so we convert The Point Temp into a Vector so we can
  // do a dot product on it
  ngl::Vec3 p;
  // D is the distance of the Agent from the Plane. If it is less than ext[i] then there is
  // no collision
  GLfloat D;
  // Loop for each sphere in the vector list
    p=m_curBoid->getPos();
    //Now we need to check the Sphere agains all 6 planes of the BBOx
    //If a collision is found we change the dir of the Sphere then Break
    for(int i=0; i<6; ++i)
    {
      //to calculate the distance we take the dotporduct of the Plane Normal
      //with the new point P
      GLfloat dist;
      dist=m_bbox->getNormalArray()[i].dot(p);
      //Now Add the Radius of the sphere to the offsett
      D=dist+(m_curBoid->getRadius())*40;
      // If this is greater or equal to the BBox extent /2 then there is a collision
      //So we calculate the Boids new direction
      if(D >=ext[i])
      {
        //We use the same calculation as in raytracing to determine the new direction
        GLfloat x= 2*( m_curBoid->getDirection().dot((m_bbox->getNormalArray()[i])));
        ngl::Vec3 d =m_bbox->getNormalArray()[i]*x;
        ngl::Vec3 avoidFactor;
        ngl::Random *rng=ngl::Random::instance();
        //to avoid having them move in 90 degrees after a few collissions
        ngl::Vec3 randNumber = rng->getRandomPoint(.002,.002,.002);
        //calculate the avoidfactor as a ratio of the distance from edge and random motion
        avoidFactor = (-1)*(randNumber+(m_bbox->getNormalArray()[i]*(dist*.0002)));
        m_curBoid->setAvoidFactor(avoidFactor*(m_repulsion*1));

      }
     }
    }

//----------------------------------------------------------------------------------------------------------------------



void DivideSpace::BBoxCollision()
{
  //create an array of the extents of the bounding box
  float ext[6];
  ext[0]=ext[1]=(m_bbox->height()/2.0f);
  ext[2]=ext[3]=(m_bbox->width()/2.0f);
  ext[4]=ext[5]=(m_bbox->depth()/2.0f);
  // Dot product needs a Vector so we convert The Point Temp into a Vector so we can
  // do a dot product on it
  ngl::Vec3 p;
  // D is the distance of the Agent from the Plane. If it is less than ext[i] then there is
  // no collision
  GLfloat D;
  // Loop for each sphere in the vector list
    p=m_curBoid->getPos();
    //Now we need to check the Sphere agains all 6 planes of the BBOx
    //If a collision is found we change the dir of the Sphere then Break
    for(int i=0; i<6; ++i)
    {
      //to calculate the distance we take the dotporduct of the Plane Normal
      //with the new point P
      D=m_bbox->getNormalArray()[i].dot(p);
      //Now Add the Radius of the sphere to the offsett
      D+=m_curBoid->getRadius();
      // If this is greater or equal to the BBox extent /2 then there is a collision
      //So we calculate the Spheres new direction
      if(D >=ext[i])
      {
        //We use the same calculation as in raytracing to determine the
        // the new direction
        GLfloat x= 2*( m_curBoid->getDirection().dot((m_bbox->getNormalArray()[i])));
        ngl::Vec3 d =m_bbox->getNormalArray()[i]*x;
        m_curBoid->setDirection(m_curBoid->getDirection()-d);
        m_curBoid->setHit();
      }
     }
    }


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
