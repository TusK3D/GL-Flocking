#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "World.h"


// \brief This is the MainWindow Class which is generated by the Ui file, if we wish to add anything to the main
// Ui we add it here
namespace Ui{
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
protected :
    /// \brief override the keyPressEvent inherited from QObject so we can handle key presses.
    /// @param [in] _event the event to process
    void keyPressEvent(
                       QKeyEvent *_event
                      );
    void resizeEvent (
                      QResizeEvent * _event
                     );
public:
    // ctor
    MainWindow(
                int _numSpheres, int argc,
                char *argv[],QWidget *parent = 0
               );    




    // dtor
    ~MainWindow();
    /// \brief here we add our GLWindow extending the base functionality of our class
private slots :
    void setMaxSpeed();
    void setCohesionFactor();
    void setSeparationFactor();
    void setAlignFactor();
    void addPredator();
    void editFlockCount();
    void editShapeType();
    void changeObstacleCount();
    void setRepulsionFactor();
    void setObsAvoid();
    void setNeighbourRadius();
    void setNeighbourPercent();
    void setGridSizex();
    void setGridSizey();
    void setGridSizez();
    void setWallAvoidStrength();
    void setDrawBBox(int _value);
    void setCurrentShader();
    void setKeepApartRange();
    void setseekRange();
    void setPredatorSeek();
    void setPreyFlee();
    void setObsAvoidRadius();







private:
    /// @brief our gl window created in world.h
    World *m_gl;
    /// @brief the UI for our window
    Ui::MainWindow *m_ui;
    /// @brief to access the values in the flock class
    Flocking *m_flock;




};

#endif // MAINWINDOW_H