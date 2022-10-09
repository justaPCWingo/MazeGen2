#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _animTimer(this)
{
    ui->setupUi(this);

    //wire additional signals
    connect(ui->gridColorwell,&ColorWellButton::colorChanged,ui->mazeGLWidget,&MazeGLView::setGridColor);
    connect(ui->wallColorwell,&ColorWellButton::colorChanged,ui->mazeGLWidget,&MazeGLView::setWallColor);
    connect(ui->pathColorwell,&ColorWellButton::colorChanged,ui->mazeGLWidget,&MazeGLView::setPathColor);
    connect(ui->decayColorwell,&ColorWellButton::colorChanged,ui->mazeGLWidget,&MazeGLView::setDecayColor);
    connect(ui->fullPathCB,&QCheckBox::toggled,ui->mazeGLWidget,&MazeGLView::setDrawsPath);
    connect(ui->showDecayCB,&QCheckBox::toggled,ui->mazeGLWidget,&MazeGLView::setShowsDecay);
    connect(ui->applyRotateCB,&QCheckBox::toggled,ui->mazeGLWidget,&MazeGLView::setRotates);
    connect(ui->decaySpinner,SIGNAL(valueChanged(int)),ui->mazeGLWidget,SLOT(setDecayDelay(int)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateAnimation()
{
    ui->mazeGLWidget->updateAnimation();
}



void MainWindow::showEvent(QShowEvent *event)
{
    ui->gridColorwell->setColor(ui->mazeGLWidget->gridColor());
    ui->wallColorwell->setColor(ui->mazeGLWidget->wallColor());
    ui->pathColorwell->setColor(ui->mazeGLWidget->pathColor());
    ui->decayColorwell->setColor(ui->mazeGLWidget->decayColor());
    ui->fullPathCB->setChecked(ui->mazeGLWidget->drawsPath());
    ui->showDecayCB->setChecked(ui->mazeGLWidget->showsDecay());
    ui->applyRotateCB->setChecked(ui->mazeGLWidget->rotates());
    ui->decaySpinner->setValue(ui->mazeGLWidget->decayDelay());


    connect(&_animTimer,&QTimer::timeout,this,&MainWindow::updateAnimation);
    _animTimer.start(1000/60);
    QMainWindow::showEvent(event);
}

void MainWindow::on_refreshButton_clicked()
{
    ui->mazeGLWidget->refreshMaze();

}

