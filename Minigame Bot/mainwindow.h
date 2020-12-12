#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include "api/ayugraapi.h"
#include "minigame.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PB_CONNECT_clicked();
    void on_PB_START_clicked();

private:
    Ui::MainWindow *ui;
    Minigame *minigame;
    AyugraApi *aApi;
};
#endif // MAINWINDOW_H
