# Ayugra PacketLogger

Since Ayugra Premium is already hooking Nostale's packet system, we can not use BladeTigger's one. We needed a way to integrate a packetlogger in Ayugra. I thought it could be better to introduce this system.

## How does it works ?

Short answer : https://github.com/ApourtArtt/Ayugra-PacketLogger/blob/main/ayugraapi.cpp \
Long answer :
Ayugra creates a Named Pipe server listening on AyugraPacketApi_{process_id}. It can handles a maximum of 8 clients. This limit should probably never be reached.
This software create a Named Pipe socket.
Ayugra can send and read to this second and vice versa. Ayugra is sending packets from NosTale to the named pipe sockets and the named pipe sockets can also send game packet.

Ayugra is sending datas that look like `<packet one\n<packet two\n>packet three\00`\
< means "sent packet" (like walk, select, ...)\
\> means "received packet" (like mv, in, ...)\
the same system appears on this software :
```cpp
bool AyugraApi::SendAsSend(QString Packet)
{
    if (!IsConnected())
        return false;
    return socket->write(QString("<" + Packet).toStdString().c_str()) > 0;
} //                             ^^^

bool AyugraApi::SendAsReceive(QString Packet)
{
    if (!IsConnected())
        return false;
    return socket->write(QString(">" + Packet).toStdString().c_str()) > 0;
} //                             ^^^
```

## And now ?

And now, you can do like some users are doing with BladeTigger's system : you can do bot, script, whatever you want.\
You have a really complete exemple of how to interact with Ayugra, and so with Nostale.

![Image of Ayugra PL](https://zupimages.net/up/20/50/zwrs.png)
Download here : https://github.com/ApourtArtt/Ayugra-PacketLogger/releases/tag/1.0
