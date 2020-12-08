#include "packettab.h"
#include "ui_packettab.h"

FilterForm *PacketTab::filterForm;

PacketTab::PacketTab(int tabNumber, QWidget *parent)
    : ui(new Ui::PacketTab)
    , parent(parent)
    , tab(tabNumber)
{
    ui->setupUi(this);

    if (filterForm == nullptr)
        filterForm = new FilterForm();

    aApi = new AyugraApi(-1, this);
    connect(aApi, &AyugraApi::onSocketConnection, [=]{ ui->PB_CONNECT->setText("Disconnect"); });
    connect(aApi, &AyugraApi::onSocketDisconnection, [=]{ aApi->Disconnect(); ui->PB_CONNECT->setText("Connect"); });
    connect(aApi, &AyugraApi::onSocketError, [=](QLocalSocket::LocalSocketError error)
    {
        if (error == QLocalSocket::SocketAccessError)
            emit onSocketError("Start as Administrator.");
        else if (error == QLocalSocket::ServerNotFoundError)
            emit onSocketError("Cannot find AyugraId (" + ui->LE_ID->text() + ").");
        else
            emit onSocketError("Something went wrong (" + ui->LE_ID->text() + ").");
    });
    connect(aApi, &AyugraApi::onSentPacket, [=](QString packet)
    {
        if (filterForm->ShouldFilterSend())
        {
            QVector<QPair<QString, FilteringType>> filter = filterForm->filterSend;
            for (int i = 0 ; i < filter.size() ; i++)
            {
                if (filter[i].second == FilteringType::PacketName && packet.startsWith(filter[i].first))
                    return;
                if (filter[i].second == FilteringType::Contains && packet.contains(filter[i].first))
                    return;
                if (filter[i].second == FilteringType::EndsWith && packet.endsWith(filter[i].first))
                    return;
            }
        }
        auto textCursor = ui->TB_PACKETS->textCursor();
        textCursor.movePosition(QTextCursor::End);
        ui->TB_PACKETS->insertPlainText(packet + "\n");
        if (ui->CB_BOTTOM->isChecked())
            ui->TB_PACKETS->setTextCursor(textCursor);
    });
    connect(aApi, &AyugraApi::onReceivedPacket, [=](QString packet)
    {
        if (filterForm->ShouldFilterRcvd())
        {
            QVector<QPair<QString, FilteringType>> filter = filterForm->filterRcvd;
            for (int i = 0 ; i < filter.size() ; i++)
            {
                if (filter[i].second == FilteringType::PacketName && packet.startsWith(filter[i].first))
                    return;
                if (filter[i].second == FilteringType::Contains && packet.contains(filter[i].first))
                    return;
                if (filter[i].second == FilteringType::EndsWith && packet.endsWith(filter[i].first))
                    return;
            }
        }
        auto textCursor = ui->TB_PACKETS->textCursor();
        textCursor.movePosition(QTextCursor::End);
        ui->TB_PACKETS->insertPlainText(packet + "\n");
        if (ui->CB_BOTTOM->isChecked())
            ui->TB_PACKETS->setTextCursor(textCursor);
    });
}

PacketTab::~PacketTab()
{
    delete ui;
    delete aApi;
}

void PacketTab::on_PB_CONNECT_clicked()
{
    if (!aApi->IsConnected())
        aApi->Connect(ui->LE_ID->text().toInt());
    else
        aApi->Disconnect();
}

void PacketTab::on_LE_ID_textChanged(const QString &arg1)
{
    emit idChanged(tab, arg1);
}

void PacketTab::on_PB_FILTER_PACKET_clicked()
{
    filterForm->show();
}

void PacketTab::on_PB_SEND_PACKET_clicked()
{
    if (ui->CB_PACKET_TYPE->isChecked())
        aApi->SendAsReceive(ui->LE_PACKET->text());
    else
        aApi->SendAsSend(ui->LE_PACKET->text());
}

void PacketTab::on_PB_CLEAR_clicked()
{
    ui->TB_PACKETS->clear();
}
