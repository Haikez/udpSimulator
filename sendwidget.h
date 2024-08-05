#ifndef SENDWIDGET_H
#define SENDWIDGET_H

#include <QWidget>
#include "protocolParser.h"
#include "databaseManager.h"
namespace Ui {
class SendWidget;
}

class SendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SendWidget(QWidget *parent = nullptr);
    ~SendWidget();

private slots:
    void on_selectXMLBtn_clicked();
    void on_sendBtn_clicked();
    void sendPacket();
    void on_stopBtn_clicked();

private:
    Ui::SendWidget *ui;
    Protocol protocol;
    DatabaseManager db;
    QTimer *timer;
    QByteArray data;
    uint totalPackets;
    uint packetCount;
    uint frequency;
    QString startTime;
    QString endTime;
};

#endif // SENDWIDGET_H
