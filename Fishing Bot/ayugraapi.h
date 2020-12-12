#ifndef AYUGRAAPI_H
#define AYUGRAAPI_H

#include <QObject>
#include <QLocalSocket>

class AyugraApi : public QObject
{
    Q_OBJECT
public:
    explicit AyugraApi(int AyugraId = -1, QObject *parent = nullptr);

    bool Connect(int AyugraId = -1); /// Return false if Ayugra Id is invalid or if socket is already connected
    void Disconnect(); /// Disconnect the named pipe socket
    bool IsConnected() const; /// Is the named pipe connected to Ayugra ?

    bool SendAsSend(QString Packet); /// Send a packet to Nostale server as a sent packet
    bool SendAsReceive(QString Packet); /// Simulate a received packet to Nostale client
    void Flush(); /// Flush the socket

signals:
    void onReceivedPacket(QString packet); /// Nostale server sent a packet to Nostale client
    void onSentPacket(QString packet); /// Nostale client sent a packet to Nostale server
    void onReceivedData(QByteArray data); /// Both combined and untreated

    void onSocketConnection(); /// Emitted when the named pipe connects to server
    void onSocketDisconnection(); /// Emitted when the named pipe disconnects from server
    void onSocketError(QLocalSocket::LocalSocketError); /// Emitted on named pipe socket error

private:
    int ayugraId;
    QLocalSocket *socket;
};

#endif // AYUGRAAPI_H
