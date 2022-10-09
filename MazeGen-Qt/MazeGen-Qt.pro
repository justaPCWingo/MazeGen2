QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES+=ASSERT_GL_ERR ASSERT_GL_GO_NUCLEAR

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS+= \
    -lGLEW \
    -lGL \

SOURCES += \
    ../VisualizationCode/ShaderMgr.cpp \
    ../VisualizationCode/VisMgr.cpp \
    ../MazeGen2/MazeBuilder.cpp \
    ColorWellButton.cpp \
    MazeGLView.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    ../VisualizationCode/GLErrStream.h \
    ../VisualizationCode/ShaderMgr.h \
    ../VisualizationCode/VisMgr.h \
    ../MazeGen2/MazeBuilder.h \
    ColorWellButton.h \
    MainWindow.h \
    MazeGLView.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../VisualizationCode/shaders/combine.frag \
    ../VisualizationCode/shaders/composite.frag \
    ../VisualizationCode/shaders/composite.vert \
    ../VisualizationCode/shaders/grid.geom \
    ../VisualizationCode/shaders/passThru.frag \
    ../VisualizationCode/shaders/passThru.vert \
    ../VisualizationCode/shaders/path.frag \
    ../VisualizationCode/shaders/path.geom \
    ../VisualizationCode/shaders/path.vert \
    ../VisualizationCode/shaders/transfer.frag \
    ../VisualizationCode/shaders/wall-lines.geom \
    ../VisualizationCode/shaders/wall.frag \
    ../VisualizationCode/shaders/wall.geom \
    ../VisualizationCode/shaders/wall.vert \
    ../README.md \
    ../convolution_matrix.py
