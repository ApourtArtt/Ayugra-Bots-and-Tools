#ifndef MINIGAME_H
#define MINIGAME_H

#include <QObject>
#include <QTimer>

enum class Status
{
    STARTED,
    STOPPED
};

class Minigame : public QObject
{
    Q_OBJECT
public:
    explicit Minigame(QObject *parent = nullptr);

    void Configure(int MinPoint, int MaxPoint, int MinTime, int MaxTime, int Box);

    void Handle_mlo_info(QStringList packets);
    void Handle_mlpt(QStringList packets);
    void Handle_mlo_st(QStringList packets);
    void Handle_mlo_lv(QStringList packets);

    void Start();
    void Stop();
    Status GetStatus() const { return status; }

signals:
    void send(QString packet);
    void resetTimer(int value);

private:
    QTimer *timer;
    Status status;

    int minPoint, maxPoint;
    int minTime, maxTime;
    int box;

    QVector<int> boxesPoint;
    int minigameId, minigameNum;
    int production, nbGamePlayed;
};

#endif // MINIGAME_H
