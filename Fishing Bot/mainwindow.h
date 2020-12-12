#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ayugraapi.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class Status
{
    STARTED = 0,
    STOPPED = 1
};

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
    void fish();
    Ui::MainWindow *ui;
    AyugraApi *aApi;
    int characterId;
    Status status;
};
#endif // MAINWINDOW_H
