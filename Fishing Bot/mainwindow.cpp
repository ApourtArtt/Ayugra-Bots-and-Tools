#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , characterId(-1)
    , status(Status::STOPPED)
{
    qsrand(static_cast<uint>(QTime::currentTime().msecsSinceStartOfDay()));
    ui->setupUi(this);

    aApi = new AyugraApi(-1, this);
    connect(aApi, &AyugraApi::onSocketConnection, [=]{ ui->PB_CONNECT->setText("Disconnect"); });
    connect(aApi, &AyugraApi::onSocketDisconnection, [=]{ aApi->Disconnect(); ui->PB_CONNECT->setText("Connect"); });
    connect(aApi, &AyugraApi::onSocketError, [=](QLocalSocket::LocalSocketError error)
    {
        if (error == QLocalSocket::SocketAccessError)
            ui->statusbar->showMessage("Start as Administrator.");
        else if (error == QLocalSocket::ServerNotFoundError)
            ui->statusbar->showMessage("Cannot find AyugraId (" + ui->LE_ID->text() + ").", 5000);
        else
            ui->statusbar->showMessage("Something went wrong (" + ui->LE_ID->text() + ").", 5000);
    });
    connect(aApi, &AyugraApi::onSentPacket, [=](QString packet)
    {
        (void)packet;
    });
    connect(aApi, &AyugraApi::onReceivedPacket, [=](QString packet)
    {
        QStringList packs = packet.split(" ", Qt::SkipEmptyParts);
        if (packs[0] == "at")
        {
            characterId = packs[1].toInt();
            ui->L_CHAR_ID->setText(QString::number(characterId));
        }
        else if (packs[0] == "guri" && packs[1] == "6" && packs[3].toInt() == characterId)
        {
            if (packs[4] == "30" || packs[4] == "31")
                fish();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PB_CONNECT_clicked()
{
    if (!aApi->IsConnected())
        aApi->Connect(ui->LE_ID->text().toInt());
    else
        aApi->Disconnect();
}

void MainWindow::on_PB_START_clicked()
{
    if (status == Status::STOPPED)
    {
        if (characterId != -1)
        {
            aApi->SendAsSend("u_s 1 1 " + QString::number(characterId));
            status = Status::STARTED;
            ui->PB_START->setText("Stop");
        }
    }
    else
    {
        status = Status::STOPPED;
        ui->PB_START->setText("Start");
    }
}

void MainWindow::fish()
{
    int timer = 200 + qrand() % 365;
    QTimer::singleShot(timer, [=]
    {
        aApi->SendAsSend("u_s 2 1 " + QString::number(characterId));
    });
    if (status == Status::STARTED)
    {
        QTimer::singleShot(timer + 7500 + qrand() % 2000, [=]
        {
            aApi->SendAsSend("u_s 1 1 " + QString::number(characterId));
        });
    }
}
