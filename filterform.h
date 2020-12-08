#ifndef FILTERFORM_H
#define FILTERFORM_H

#include <QWidget>
#include <QVector>

enum class FilteringType
{
    PacketName  = 0,
    Contains    = 1,
    EndsWith    = 2
};

namespace Ui {
class FilterForm;
}

class FilterForm : public QWidget
{
    Q_OBJECT

public:
    explicit FilterForm(QWidget *parent = nullptr);
    ~FilterForm();

    bool ShouldFilterSend() const;
    bool ShouldFilterRcvd() const;
    QVector<QPair<QString, FilteringType>> filterSend, filterRcvd;

private slots:
    void on_PB_SEND_FILTER_clicked();
    void on_PB_RCVD_FILTER_clicked();
    void on_PB_SEND_DELETE_clicked();
    void on_PB_RCVD_DELETE_clicked();

private:
    void loadFilters();
    void saveFilters();

    Ui::FilterForm *ui;
    const QString FILTER_FILENAME = "packetlogger_filter.txt";
};

#endif // FILTERFORM_H
