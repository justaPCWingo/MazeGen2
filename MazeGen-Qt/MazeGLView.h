#pragma once
#include <GL/glew.h>
#include <QOpenGLWidget>
#include "../VisualizationCode/VisMgr.h"
#include "../MazeGen2/MazeBuilder.h"

class MazeGLView : public QOpenGLWidget
{
    Q_OBJECT
public:
    MazeGLView(QWidget* parent);
    virtual ~MazeGLView();

    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor)
    Q_PROPERTY(QColor wallColor READ wallColor WRITE setWallColor)
    Q_PROPERTY(QColor pathColor READ pathColor WRITE setPathColor)
    Q_PROPERTY(QColor decayColor READ decayColor WRITE setDecayColor)
    Q_PROPERTY(bool drawsPath READ drawsPath WRITE setDrawsPath)
    Q_PROPERTY(bool showsDecay READ showsDecay WRITE setShowsDecay)
    Q_PROPERTY(int decayDelay READ decayDelay WRITE setDecayDelay)
    Q_PROPERTY(bool rotates READ rotates WRITE setRotates)

    // QOpenGLWidget interface
    QColor gridColor() const;
    QColor wallColor() const;
    QColor pathColor() const;
    QColor decayColor() const;
    bool drawsPath() const;
    bool showsDecay() const;
    int decayDelay() const;
    bool rotates() const;

    void updateAnimation();
    void refreshMaze();

public slots:
    void setDrawsPath(const bool & draw);
    void setGridColor(const QColor & color);
    void setWallColor(const QColor & color);
    void setPathColor(const QColor & color);
    void setDecayColor(const QColor & color);
    void setShowsDecay(const bool & show);
    void setDecayDelay(const int & delay);
    void setRotates(const bool & rotate);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;


    VisMgr* _pVisMgr;
    MazeBuilder _mazeBldr;

};

