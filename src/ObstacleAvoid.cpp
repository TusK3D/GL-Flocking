#include "ObstacleAvoid.h"


ObstacleAvoid::ObstacleAvoid(ngl::Vec3 _pos, float _obsRad,
                                float _avoidRad)
{
  m_obsRad=_obsRad;
  m_avoidRad=_avoidRad;
  m_pos=_pos;
  m_inRange = 0;
}

void ObstacleAvoid::draw(const std::string &_shaderName,
                          ngl::TransformStack &_transformStack, ngl::Camera *_cam) const
{

  if(m_inRange)
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }


  ngl::VAOPrimitives *m_obstacle=ngl::VAOPrimitives::instance();

  ngl::ShaderLib *obsShader=ngl::ShaderLib::instance();
  obsShader->use(_shaderName);
  if (_shaderName == "Phong")
  {
    ngl::Material m(ngl::BRONZE );
    m.loadToShader("material");
  }
  else
  {
    obsShader->setShaderParam4f("Colour",.6,0,.2,1.0);
  }

  _transformStack.pushTransform();
  {

    _transformStack.setPosition(m_pos);
    _transformStack.setScale(1,1,1);
    loadMatricesToShader(_transformStack,_cam, _shaderName);
    m_obstacle->createSphere("sphere",m_obsRad,12.0);
    m_obstacle->draw("sphere");
  } // and before a pop
   _transformStack.popTransform();

}

void ObstacleAvoid::loadMatricesToShader(
                                     ngl::TransformStack &_tx,
                                     ngl::Camera *_cam,
                                     std::string const &_shaderName

                                   ) const
{
  ngl::ShaderLib *obsShader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
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
    obsShader->setShaderParamFromMat4("MV",MV);
    obsShader->setShaderParamFromMat3("normalMatrix",normalMatrix);
    obsShader->setShaderParamFromMat4("M",M);
  }
  obsShader->setShaderParamFromMat4("MVP",MVP);
}

ngl::Vec3 ObstacleAvoid::obstacleForce(Boid const & _focusBoid, float const _maxSpeed, bool &_contact, float &_obsdist)
{
  m_inRange = false;
  ngl::Vec3 _dist(0);
  _dist = m_pos-_focusBoid.getPos();

  if (_dist != 0 )
  {
    float _diff= _dist.length();

    if(_diff != 0)
    {
    if (_diff < (m_avoidRad+_focusBoid.getRadius()))
    {
      if(_obsdist < _diff)
      {
        _obsdist = _diff;
        if (_diff > (m_obsRad+_focusBoid.getRadius()))
          {
            m_inRange = true;
            _dist.normalize();
            _dist = _dist/_diff*.002;
            _dist.normalize();
            _dist*=_maxSpeed;
            _dist=_dist-_focusBoid.getVel();
            return _dist;
          }
      else
        if (_diff == (m_obsRad+_focusBoid.getRadius()))
          {
            _contact = 1;
            return 0;
          }
      }
      }
    }
  }
  return 0;
}
