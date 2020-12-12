#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
        if (packs[0] == "mlo_info")
            minigame->Handle_mlo_info(packs);
        else if (packs[0] == "mlpt")
            minigame->Handle_mlpt(packs);
        else if (packs[0] == "mlo_lv")
            minigame->Handle_mlo_lv(packs);
        else if (packs[0] == "mlo_st")
            minigame->Handle_mlo_st(packs);
    });

    minigame = new Minigame(this);
    connect(minigame, &Minigame::send, [=](QString packet)
    {
        qDebug() << "Sending : " << packet;
        aApi->SendAsSend(packet);
    });
    connect(minigame, &Minigame::resetTimer, [=](int timer)
    {
        QTimer *pb = new QTimer(this);
        int proc = 0;
        connect(pb, &QTimer::timeout, [=]() mutable
        {
            int value = static_cast<float>(static_cast<float>(proc) / static_cast<float>(timer) * 100000);
            ui->PB_STATUS_BAR->setValue(value);
            proc++;
        });
        pb->start(1000);
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
    // check Minigame::GetStatus()
    if (minigame->GetStatus() == Status::STARTED)
    {
        minigame->Stop();
        ui->PB_START->setText("Start");
    }
    else
    {
        minigame->Configure(ui->LE_MIN_P->text().toInt(), ui->LE_MAX_P->text().toInt(),
                            ui->LE_MIN_T->text().toInt(), ui->LE_MAX_T->text().toInt(),
                            ui->CB_BOX->currentIndex());
        minigame->Start();
        ui->PB_START->setText("Stop");
    }
}
