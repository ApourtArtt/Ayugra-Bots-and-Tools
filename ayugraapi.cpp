#include "ayugraapi.h"

AyugraApi::AyugraApi(int AyugraId, QObject *parent)
    : QObject(parent)
    , ayugraId(AyugraId)
{
    socket = new QLocalSocket(this);
    connect(socket, &QLocalSocket::readyRead, [=]
    {
        QStringList packets = QString(socket->readAll()).split("\n", Qt::SkipEmptyParts);
        for (int i = 0 ; i < packets.size() ; i ++)
        {
            if (packets[i].startsWith("<")) // Sent packet
                emit onSentPacket(packets[i].remove(0, 1));
            else if (packets[i].startsWith(">")) // Rcvd packet
                emit onReceivedPacket(packets[i].remove(0, 1));
            else // Error coming from Ayugra
                emit onSocketError(QLocalSocket::UnknownSocketError);
        }
    });
    connect(socket, &QLocalSocket::connected, [=]() mutable
    {
        emit onSocketConnection();
    });
    connect(socket, &QLocalSocket::disconnected, [=]
    {
        emit onSocketDisconnection();
    });
    connect(socket, qOverload<QLocalSocket::LocalSocketError>(&QLocalSocket::error),
            [=](QLocalSocket::LocalSocketError error)
    {
        emit onSocketError(error);
    });
}

bool AyugraApi::Connect(int AyugraId)
{
    if (AyugraId != -1)
        ayugraId = AyugraId;
    if (ayugraId == -1 || socket->isOpen())
        return false;
    socket->connectToServer("AyugraPacketApi_" + QString::number(ayugraId));
    return true;
}

void AyugraApi::Disconnect()
{
    if (IsConnected())
        socket->disconnectFromServer();
}

bool AyugraApi::IsConnected() const
{
    return socket->isOpen();
}

bool AyugraApi::SendAsSend(QString Packet)
{
    if (!IsConnected())
        return false;
    return socket->write(QString("<" + Packet).toStdString().c_str()) > 0;
}

bool AyugraApi::SendAsReceive(QString Packet)
{
    if (!IsConnected())
        return false;
    return socket->write(QString(">" + Packet).toStdString().c_str()) > 0;
}

void AyugraApi::Flush()
{
    if (IsConnected())
        socket->flush();
}
