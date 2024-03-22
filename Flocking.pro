# -------------------------------------------------
# Project created by QtCreator 2009-11-05T22:11:46
# -------------------------------------------------
QT += opengl
QT += core \
    gui \
    xml

TARGET=Flocking
DESTDIR=./
OBJECTS_DIR=obj
# this is where we want to put the intermediate build files ( .o)
OBJECTS_DIR=./obj/
MOC_DIR=./moc/
SOURCES += src/main.cpp \
           src/MainWindow.cpp \
    src/Boid.cpp \
    src/World.cpp \
    src/Flocking.cpp \
    src/DivideSpace.cpp \
    src/ObstacleAvoid.cpp \
    src/Parser.cpp


INCLUDEPATH+=./include \
            +=ui

HEADERS += include/MainWindow.h \
    include/Boid.h \
    include/World.h \
    include/Flocking.h \
    include/DivideSpace.h \
    include/ObstacleAvoid.h \
    include/Parser.h

OTHER_FILES+= shaders/Fragment.fs \
              shaders/Vertex.vs \
    shaders/Constant.vs \
    shaders/Constant.fs \
    shaders/Phong.vs \
    shaders/Phong.fs

CONFIG += console
CONFIG -= app_bundle
#UI_HEADERS_DIR=ui
QMAKE_CXX=clang++  -fdiagnostics-fixit-info
DEFINES+=GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG

LIBS += -L/usr/local/lib
# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-clang {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
linux-g++-64 {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN

# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/

win32: {
        DEFINES+=USING_GLEW
        INCLUDEPATH+=-I c:/boost_1_44_0
        INCLUDEPATH+=-I c:/boost

        INCLUDEPATH+= -I C:/NGL/Support/glew
        LIBS+= -L C:/NGL/lib
        LIBS+= -lmingw32
        DEFINES += WIN32
        DEFINES += USING_GLEW
        DEFINES +=GLEW_STATIC
        DEFINES+=_WIN32
        SOURCES+=C:/NGL/Support/glew/glew.c
        INCLUDEPATH+=C:/NGL/Support/glew/
}

FORMS += \
    ui/MainWindow.ui \

