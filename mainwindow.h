#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "protocolParser.h"
#include "databaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectXMLBtn_clicked();
    void on_sendBtn_clicked();
    void sendPacket();
    void on_stopBtn_clicked();

private:
    Ui::MainWindow *ui;
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

#endif // MAINWINDOW_H
