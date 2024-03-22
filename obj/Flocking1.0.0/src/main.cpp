#include <QtGui/QApplication>
#include "MainWindow.h"


/* this code runs the basic main window and is created by the Qt Creator app */
int main(int argc, char *argv[])
{
  // make an instance of the QApplication
  QApplication a(argc, argv);
  // Create a new MainWindow
  int numBoids;
  if(argc ==2)
  {
        numBoids=500;
  }
  else
  {
      numBoids=atoi(argv[1]);
  }
  MainWindow w(numBoids, argc,argv);
  // show it
  w.show();
  // hand control over to Qt framework
  return a.exec();
}

