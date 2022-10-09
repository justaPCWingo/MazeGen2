#pragma once

#include <QMainWindow>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer _animTimer;

    void updateAnimation();
    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
private slots:
    void on_refreshButton_clicked();
};
