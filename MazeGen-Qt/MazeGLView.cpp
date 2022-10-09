#include "MazeGLView.h"
#include "../VisualizationCode/GLErrStream.h"
#include <random>
#include <chrono>
#include <cassert>
#include <glm/glm.hpp>

inline QColor GLMToQColor(const glm::vec3 & color)
{
    return QColor::fromRgbF(color.r,color.g,color.b);
}

inline glm::vec3 QColorToGLM(const QColor & color)
{
    return glm::vec3(color.redF(),color.greenF(),color.blueF());
}
//===============================================================================


MazeGLView::MazeGLView(QWidget *parent)
    :QOpenGLWidget(parent)
    ,_pVisMgr(nullptr)
    ,_mazeBldr(10,10)
{
    QSurfaceFormat format;
    format.setVersion(4,0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    setFormat(format);
}

MazeGLView::~MazeGLView()
{
    delete _pVisMgr;
}

void MazeGLView::initializeGL()
{
    // enable drivers
    glewExperimental=GL_TRUE;
    glewInit();

    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);				// Black background
    ASSERT_GL_WRAP(glClearDepth(1.0f));									// Depth buffer setup

    ASSERT_GL_WRAP(glEnable(GL_DEPTH_TEST));                            // Enables depth testing
    ASSERT_GL_WRAP(glDepthFunc(GL_LEQUAL));								// The type of depth testing to do
    //ASSERT_GL_WRAP(glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST));

    //initialize visMgr
    _pVisMgr = new VisMgr("shaders",0,0,this->width(),this->height());
    _pVisMgr->InitForOpenGL();
    _pVisMgr->NewMaze(&_mazeBldr);

    refreshMaze();
}

void MazeGLView::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    if (_pVisMgr)
        _pVisMgr->SetViewport(0,0,w,h);
}

void MazeGLView::paintGL()
{
    if(_pVisMgr && _pVisMgr->ReadyToDraw())
        _pVisMgr->Draw();
    else
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void MazeGLView::refreshMaze()
{
    _mazeBldr.GenerateMaze();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> distrib(-0.1,0.1);
    assert(_pVisMgr!=nullptr);

    makeCurrent();
    _pVisMgr->RefreshWithMaze(&_mazeBldr);
    _pVisMgr->SetPathTime(0.);
    _pVisMgr->SetRotAxis(distrib(generator),distrib(generator),distrib(generator));
    doneCurrent();
    this->update();

}

QColor MazeGLView::gridColor() const
{
    if (_pVisMgr)
        return GLMToQColor(_pVisMgr->GetGridColor());
    return QColor(127,0,0);
}

void MazeGLView::setGridColor(const QColor & color)
{
    if(_pVisMgr)
        _pVisMgr->SetGridColor(QColorToGLM(color));
}

QColor MazeGLView::wallColor() const
{
    if (_pVisMgr)
        return GLMToQColor(_pVisMgr->GetWallColor());
    return QColor(255,0,0);
}


void MazeGLView::setWallColor(const QColor & color)
{
    if(_pVisMgr)
        _pVisMgr->SetWallColor(QColorToGLM(color));
}

QColor MazeGLView::pathColor() const
{
    if (_pVisMgr)
        return GLMToQColor(_pVisMgr->GetPathColor());
    return QColor(0,255,255);
}

void MazeGLView::setPathColor(const QColor & color)
{
    if(_pVisMgr)
        _pVisMgr->SetPathColor(QColorToGLM(color));
}

QColor MazeGLView::decayColor() const
{
    if (_pVisMgr)
        return GLMToQColor(_pVisMgr->GetDecayedPathColor());
    return QColor(127,127,127);
}

void MazeGLView::setDecayColor(const QColor & color)
{
    if(_pVisMgr)
        _pVisMgr->SetDecayedPathColor(QColorToGLM(color));
}


bool MazeGLView::drawsPath() const
{
    if(_pVisMgr)
        return _pVisMgr->GetShowFullPath();
    return true;
}

void MazeGLView::setDrawsPath(const bool & draw)
{
    if(_pVisMgr)
        _pVisMgr->SetShowFullPath(draw);
}

bool MazeGLView::showsDecay() const
{
    if(_pVisMgr)
        return _pVisMgr->GetShowPathDecay();
    return true;
}

void MazeGLView::setShowsDecay(const bool & show)
{
    if(_pVisMgr)
        _pVisMgr->SetShowPathDecay(show);
}

int MazeGLView::decayDelay() const
{
    if(_pVisMgr)
        return _pVisMgr->GetDecayDelay();
    return 2;
}

void MazeGLView::setDecayDelay(const int & delay)
{
    if(_pVisMgr)
        _pVisMgr->SetDecayDelay(delay);
}

bool MazeGLView::rotates() const
{
    if(_pVisMgr)
        return _pVisMgr->GetApplyRot();
    return false;
}

void MazeGLView::setRotates(const bool & rotate)
{
    if(_pVisMgr)
        _pVisMgr->SetApplyRot(rotate);
}

void MazeGLView::updateAnimation()
{
    if (_pVisMgr->GetPathTime()< _pVisMgr->GetPathLength()+8)
    {
        _pVisMgr->SetPathTime(_pVisMgr->GetPathTime()+0.017);
        this->update();
    }
    else
    {
        refreshMaze();
    }
}
