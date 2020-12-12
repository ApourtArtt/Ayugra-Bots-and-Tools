#include "filterform.h"
#include "ui_filterform.h"
#include <QFile>

#include <QtDebug>

FilterForm::FilterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterForm)
{
    ui->setupUi(this);
    loadFilters();
}

FilterForm::~FilterForm()
{
    delete ui;
}

bool FilterForm::ShouldFilterSend() const
{
    return ui->CB_SEND_FILTER->isChecked();
}

bool FilterForm::ShouldFilterRcvd() const
{
    return ui->CB_RCVD_FILTER->isChecked();
}

void FilterForm::on_PB_SEND_FILTER_clicked()
{
    filterSend.push_back({ ui->LE_SEND_NEW_PACKET->text(),
                           static_cast<FilteringType>(ui->CB_SEND_FILTERING_TYPE->currentIndex()) });
    ui->CB_SEND_FILTERED->insertItem(ui->CB_SEND_FILTERED->count(), ui->LE_SEND_NEW_PACKET->text());
    ui->LE_SEND_NEW_PACKET->clear();
    saveFilters();
}

void FilterForm::on_PB_RCVD_FILTER_clicked()
{
    filterRcvd.push_back({ ui->LE_RCVD_NEW_PACKET->text(),
                           static_cast<FilteringType>(ui->CB_RCVD_FILTERING_TYPE->currentIndex()) });
    ui->CB_RCVD_FILTERED->insertItem(ui->CB_RCVD_FILTERED->count(), ui->LE_RCVD_NEW_PACKET->text());
    ui->LE_RCVD_NEW_PACKET->clear();
    saveFilters();
}

void FilterForm::on_PB_SEND_DELETE_clicked()
{
    int index = ui->CB_SEND_FILTERED->currentIndex();
    if (index >= filterSend.size())
        return;
    filterSend.remove(index);
    ui->CB_SEND_FILTERED->removeItem(index);
    saveFilters();
}

void FilterForm::on_PB_RCVD_DELETE_clicked()
{
    int index = ui->CB_RCVD_FILTERED->currentIndex();
    if (index >= filterRcvd.size())
        return;
    filterRcvd.remove(index);
    ui->CB_RCVD_FILTERED->removeItem(index);
    saveFilters();
}

void FilterForm::loadFilters()
{
    QFile file(FILTER_FILENAME);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList fields = line.split("|");
        if (fields.size() == 3)
        {
            if (fields[0] == "S")
            {
                filterSend.push_back({ fields[1],
                                       static_cast<FilteringType>(fields[2].toInt()) });
                ui->CB_SEND_FILTERED->insertItem(ui->CB_SEND_FILTERED->count(), fields[1]);
            }
            else if (fields[0] == "R")
            {
                filterRcvd.push_back({ fields[1],
                                       static_cast<FilteringType>(fields[2].toInt()) });
                ui->CB_RCVD_FILTERED->insertItem(ui->CB_RCVD_FILTERED->count(), fields[1]);
            }
        }
    }
    file.close();
}

void FilterForm::saveFilters()
{
    QFile file(FILTER_FILENAME);
    if (!file.open(QIODevice::Append))
        return;
    file.resize(0);
    QTextStream stream(&file);
    for (int i = 0 ; i < filterSend.size() ; i++)
        stream << "S|" << filterSend[i].first << "|" << QString::number(static_cast<int>(filterSend[i].second)) << endl;
    for (int i = 0 ; i < filterRcvd.size() ; i++)
        stream << "R|" << filterRcvd[i].first << "|" << QString::number(static_cast<int>(filterRcvd[i].second)) << endl;
    file.close();
}
