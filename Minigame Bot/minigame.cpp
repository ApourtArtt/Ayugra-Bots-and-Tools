#include "minigame.h"
#include <QtDebug>

Minigame::Minigame(QObject *parent)
    : QObject(parent)
    , status(Status::STOPPED)
    , minPoint(-1)
    , maxPoint(-1)
    , minTime(-1)
    , maxTime(-1)
    , box(-1)
    , minigameId(-1)
    , minigameNum(-1)
    , production(-1)
    , nbGamePlayed(0)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
}

void Minigame::Configure(int MinPoint, int MaxPoint, int MinTime, int MaxTime, int Box)
{
    minPoint = MinPoint;
    maxPoint = MaxPoint;
    minTime = MinTime;
    maxTime = MaxTime;
    box = Box;
}

// popup minigame with score to do etc
void Minigame::Handle_mlo_info(QStringList packets)
{
    boxesPoint.clear();

    boxesPoint.push_back(packets[9].toInt());
    boxesPoint.push_back(packets[11].toInt());
    boxesPoint.push_back(packets[13].toInt());
    boxesPoint.push_back(packets[15].toInt());
    boxesPoint.push_back(packets[17].toInt());

    minigameId = packets[2].toInt();
    minigameNum = packets[3].toInt();

    qDebug() << "R: " << packets.join(" ");
    if (status == Status::STARTED)
    {
        timer->singleShot(500 + qrand()% 2500, [=]
        {
            emit send("mg 1 " + QString::number(minigameNum) + " " + QString::number(minigameId));
        });
    }
}

// after accepting rewards
void Minigame::Handle_mlpt(QStringList packets)
{
    production = packets[1].toInt();
    qDebug() << "R: " << packets.join(" ");
    if (nbGamePlayed <= 0 || production <= 0)
        return;

    if (status == Status::STARTED)
    {
        timer->singleShot(500 + qrand()% 1000, [=]
        {
            emit send("mg 1 " + QString::number(minigameNum) + " " + QString::number(minigameId));
        });
    }
}

// End of game : send your points
void Minigame::Handle_mlo_st(QStringList packets)
{
    qDebug() << "R: " << packets.join(" ");
    if (production == 0)
        return;
    int time = (minTime + qrand() % (maxTime - minTime)) * 1000;
    emit resetTimer(time);
    qDebug() << time;
    QTimer::singleShot(time, [=]
    {
        int point = minPoint + qrand() % (maxPoint - minPoint);
        nbGamePlayed++;
        if (point >= boxesPoint[box])
            emit send("mg 3 " + QString::number(minigameNum) + " " + QString::number(minigameId) + " " + QString::number(point) + " " + QString::number(point));
        else
            emit send("mg 1 " + QString::number(minigameNum) + " " + QString::number(minigameId));
    });
}

// get box
void Minigame::Handle_mlo_lv(QStringList packets)
{
    qDebug() << "R: " << packets.join(" ");
    QTimer::singleShot(1500 + qrand() % 2500, [=]
    {
        emit send("mg 4 " + QString::number(minigameNum) + " " + QString::number(minigameId) + " " + QString(packets[1]));
    });
}

void Minigame::Start()
{
    status = Status::STARTED;
}

void Minigame::Stop()
{
    status = Status::STOPPED;
    timer->stop();
}
