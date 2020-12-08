#ifndef PACKETTAB_H
#define PACKETTAB_H

#include <QWidget>
#include "ayugraapi.h"
#include "filterform.h"

namespace Ui {
class PacketTab;
}

class PacketTab : public QWidget
{
    Q_OBJECT

public:
    explicit PacketTab(int tabNumber, QWidget *parent = nullptr);
    ~PacketTab();

signals:
    void idChanged(int tab, QString id);
    void onSocketError(QString errorMessage);

private slots:
    void on_PB_CONNECT_clicked();
    void on_LE_ID_textChanged(const QString &arg1);
    void on_PB_FILTER_PACKET_clicked();
    void on_PB_SEND_PACKET_clicked();

    void on_PB_CLEAR_clicked();

private:
    Ui::PacketTab *ui;
    QWidget *parent;
    int tab;
    AyugraApi *aApi;
    static FilterForm *filterForm;
};

#endif // PACKETTAB_H
