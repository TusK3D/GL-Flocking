#include "MainWindow.h"
#include <ui_MainWindow.h>

MainWindow::MainWindow(
											 int _numSpheres,
                       int argc, char *argv[],
                       QWidget *_parent
                      ):
                        QMainWindow(_parent),
                        m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    this->resize(QSize(1300,720));
    m_gl = new World(_numSpheres,argc,argv,this);
    m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,5,1);
    this->setWindowTitle(QString("Flocking System _KewlaniT"));

    m_flock = new Flocking;
    m_flock = m_gl->getCurrentFlock();

    connect(m_ui->boidSpeed,SIGNAL(valueChanged(double)),this,SLOT(setMaxSpeed()));
    connect(m_ui->cohesionFactor, SIGNAL(valueChanged(double)),this,SLOT(setCohesionFactor()));
    connect(m_ui->separationFactor,SIGNAL(valueChanged(double)),this,SLOT(setSeparationFactor()));
    connect(m_ui->alignFactor,SIGNAL(valueChanged(double)),this,SLOT(setAlignFactor()));

    connect(m_ui->predatorAdd, SIGNAL(clicked()), this, SLOT(addPredator()));

    m_ui->m_boidCount->setValue(m_flock->getBoidCount());
    connect(m_ui->m_boidCount,SIGNAL(valueChanged(int)),this,SLOT(editFlockCount()));

    connect(m_ui->boidShapeType,SIGNAL(currentIndexChanged(int)),this,SLOT(editShapeType()));

    connect(m_ui->m_obstacleCount,SIGNAL(valueChanged(int)),this,SLOT(changeObstacleCount()));

    connect(m_ui->boidSpeed,SIGNAL(valueChanged(double)),this,SLOT(setRepulsionFactor()));

    connect(m_ui->m_avoidFactor,SIGNAL(valueChanged(double)),this,SLOT(setObsAvoid()));

    m_ui->m_neighbourRegionRadius->setValue(m_flock->getUserSearchRadius());
    connect(m_ui->m_neighbourRegionRadius,SIGNAL(valueChanged(double)),this,SLOT(setNeighbourRadius()));

    connect(m_ui->m_neighboursConsider,SIGNAL(valueChanged(int)),this,SLOT(setNeighbourPercent()));

    connect(m_ui->m_sizex,SIGNAL(valueChanged(int)),this,SLOT(setGridSizex()));
    connect(m_ui->m_sizey,SIGNAL(valueChanged(int)),this,SLOT(setGridSizey()));
    connect(m_ui->m_sizez,SIGNAL(valueChanged(int)),this,SLOT(setGridSizez()));

    connect(m_ui->m_wallAvoidStrength,SIGNAL(valueChanged(double)),this,SLOT(setWallAvoidStrength()));

    connect(m_ui->m_drawBBox,SIGNAL(stateChanged(int)),this,SLOT(setDrawBBox(int )));

    connect(m_ui->m_shaderName,SIGNAL(currentIndexChanged(int)),this,SLOT(setCurrentShader()));

    m_ui->m_keepApartRange->setValue(m_flock->getSeparationDistance());
    connect(m_ui->m_keepApartRange,SIGNAL(valueChanged(double)),this,SLOT(setKeepApartRange()));

    m_ui->m_seekRange->setValue(m_flock->getSeekDistance());
    connect(m_ui->m_seekRange,SIGNAL(valueChanged(double)),this,SLOT(setseekRange()));

    connect(m_ui->m_preyFleeFactor,SIGNAL(valueChanged(double)),this,SLOT(setPreyFlee()));

    connect(m_ui->m_predatorSeekFactor,SIGNAL(valueChanged(double)),this,SLOT(setPredatorseek()));

    connect(m_ui->m_obsAffectRadius,SIGNAL(valueChanged(double)),this,SLOT(setObsAvoidRadius()));






}

MainWindow::~MainWindow()
{
}


void MainWindow::keyPressEvent(
                               QKeyEvent *_event
                              )
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_A : m_gl->toggleAnimation(); break;
  case Qt::Key_F : showFullScreen(); break;
  case Qt::Key_N : showNormal(); break;
  default : break;
  }
  m_gl->keyPress(_event);
	// finally update the GLWindow and re-draw
  m_gl->updateGL();
}

void MainWindow::resizeEvent ( QResizeEvent * _event )
{
//  m_gl->resize(_event->size());
}

void MainWindow::setMaxSpeed()
{
  m_flock->setFlockMaxSpeed(m_ui->boidSpeed->value());
    m_gl->updateGL();
}

void MainWindow::setCohesionFactor()
{
  m_flock->setCohesionFactor(m_ui->cohesionFactor->value());
  m_gl->updateGL();
}

void MainWindow::setSeparationFactor()
{
  m_flock->setSepararationFactor(m_ui->separationFactor->value());
  m_gl->updateGL();
}

void MainWindow::setAlignFactor()
{
  m_flock->setAlignFactor(m_ui->alignFactor->value());
  m_gl->updateGL();
}

void MainWindow::addPredator()
{
  m_flock->addPredatorBoid();
  m_flock->m_staticPredatorCount+=1;
//  m_gl->updateGL();
}

void MainWindow::editFlockCount()
{
  m_flock->editFlockCount(m_ui->m_boidCount->value());
//  m_gl->updateGL();
}

void MainWindow::editShapeType()
{
  m_flock->setFlockShape(m_ui->boidShapeType->currentIndex());
  m_gl->updateGL();
}


void MainWindow::changeObstacleCount()
{
  {
    m_gl->createNewObstacles(m_ui->m_obstacleCount->value());
    m_gl->updateGL();
  }
}

void MainWindow::setRepulsionFactor()
{
  {
    m_gl->getCurrentGrid()->setRepulsion(m_ui->boidSpeed->value());
    m_gl->updateGL();
  }
}

void MainWindow::setObsAvoid()
{
  m_gl->getCurrentFlock()->setUserObsAvoid(m_ui->m_avoidFactor->value());
  m_gl->updateGL();
}

void MainWindow::setNeighbourRadius()
{
  m_gl->getCurrentFlock()->setUserSearchRegion(m_ui->m_neighbourRegionRadius->value());
  m_gl->updateGL();
}

void MainWindow::setNeighbourPercent()
{
  m_gl->getCurrentFlock()->setNeighbourPercent(m_ui->m_neighboursConsider->value());
  m_gl->updateGL();
}

void MainWindow::setGridSizex()
{
  m_gl->getCurrentGrid()->setContainerSizex(m_ui->m_sizex->value());
  m_gl->getCurrentGrid()->createBBox();
  m_gl->updateGL();
}

void MainWindow::setGridSizey()
{
  m_gl->getCurrentGrid()->setContainerSizey(m_ui->m_sizey->value());
  m_gl->getCurrentGrid()->createBBox();
  m_gl->updateGL();
}

void MainWindow::setGridSizez()
{
  m_gl->getCurrentGrid()->setContainerSizez(m_ui->m_sizez->value());
  m_gl->getCurrentGrid()->createBBox();
  m_gl->updateGL();
}

void MainWindow::setWallAvoidStrength()
{
  m_gl->getCurrentFlock()->setFlockWallAvoidMultiplier(m_ui->m_wallAvoidStrength->value());
//  m_gl->updateGL();
}


void MainWindow::setDrawBBox(int _value)
{
  m_gl->toggleBBoxDraw(_value);
}

void MainWindow::setCurrentShader()
{
  m_gl->setCurrentShader(m_ui->m_shaderName->currentIndex());
}

void MainWindow::setKeepApartRange()
{
  m_gl->getCurrentFlock()->setSeparationDistance(m_ui->m_keepApartRange->value());
}

void MainWindow::setseekRange()
{
  m_gl->getCurrentFlock()->setSeekDistance(m_ui->m_seekRange->value());
}

void MainWindow::setPreyFlee()
{
  m_gl->getCurrentFlock()->setUserPreyfleeFactor(m_ui->m_preyFleeFactor->value());
}

void MainWindow::setPredatorSeek()
{
  m_gl->getCurrentFlock()->setUserPredSeekFactor(m_ui->m_predatorSeekFactor->value());
}

void MainWindow::setObsAvoidRadius()
{
  for (int i=0; i < m_gl->getObstacleCount();i++)
  {
    m_gl->getCurrentObstacle(i).setavoidRad(m_ui->m_obsAffectRadius->value());
  }
}


