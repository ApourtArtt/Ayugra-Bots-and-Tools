#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLocalSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionAdd_tab, &QAction::triggered, [=](bool trigerred)
    {
        (void)trigerred;
        PacketTab *pT = new PacketTab(ui->tabWidget->count(), this);
        connect(pT, &PacketTab::idChanged, [=](int tab, QString name){ ui->tabWidget->setTabText(tab, name); });
        connect(pT, &PacketTab::onSocketError, [=](QString err){ ui->statusbar->showMessage(err, 15000); });
        ui->tabWidget->insertTab(ui->tabWidget->count(), pT, "None");
    });
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, [=](int index)
    {
        auto pT = ui->tabWidget->widget(index);
        delete pT;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
