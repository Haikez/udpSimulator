#include "sendwidget.h"
#include "ui_sendwidget.h"
#include "protocolParser.h"
#include "udpSender.h"
#include <QFileDialog>
#include <QDataStream>
#include <QByteArray>
#include <QtTypes>
#include <QDebug>
#include <QTimer>
SendWidget::SendWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SendWidget)
{
    ui->setupUi(this);
}

SendWidget::~SendWidget()
{
    delete ui;
}
void SendWidget::on_selectXMLBtn_clicked()
{
    QString fileName, filePath,filter;
    // 打开XML
    QString curPath=QCoreApplication::applicationDirPath();
    QString dlgTitle="打开文件";
    filter = "程序(*.xml);;所有(*.*)";
    filePath = QFileDialog::getOpenFileName(this,dlgTitle,"",filter);

    // 显示文件路径到编辑框
    ui->FilePathLE->setText(filePath);
    if(filePath.isEmpty()) return;

    // 开始解析XML
    ProtocolParser protocolParser;
    protocol = protocolParser.getProtocol(filePath);

    // 将读到的XML配置更新到客户端界面
    ui->sourceIPLE->setText(protocol.sourceIP);
    ui->sourcePortLE->setText(QString::number(protocol.sourcePort));
    ui->destIPLE->setText(protocol.destIP);
    ui->destPortLE->setText(QString::number(protocol.destPort));
}


void SendWidget::on_sendBtn_clicked()
{   startTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->logText->append("开始发送");
    packetCount=0;
    totalPackets=ui->numLE->text().toUInt();
    frequency=ui->freqLE->text().toUInt();

    // 定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SendWidget::sendPacket);
    timer->start(frequency);
}
void SendWidget::sendPacket(){
    // 检查是否已达到总发送包数
    if (totalPackets > 0 && packetCount >= totalPackets) {
        on_stopBtn_clicked();
        return;
    }
    // 发送udp数据报
    protocol.sourceIP=ui->sourceIPLE->text();
    protocol.sourcePort=ui->sourcePortLE->text().toUInt();
    protocol.destIP=ui->destIPLE->text();
    protocol.destPort=ui->destPortLE->text().toUInt();

    UdpSender udpSender;
    data = udpSender.serializeProtocol(protocol);
    udpSender.sending(data,QHostAddress(protocol.destIP),protocol.destPort);
    packetCount++;
}

void SendWidget::on_stopBtn_clicked()
{
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
        endTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        db.insertLog(protocol.sourceIP,protocol.sourcePort,protocol.destIP,protocol.destPort,
                     frequency,packetCount,protocol.fields.at(0).datatype,startTime,endTime);
        QString log=  "目的IP: "+protocol.destIP+"\n"+
                      "目的端口: "+QString::number(protocol.destPort)+"\n"+
                      "发送报文总数量: "+QString::number(packetCount)+"\n"+
                      "发送频率: "+QString::number(frequency)+"\n"+
                      "开始时间: "+startTime+"\n"+
                      "结束时间: "+endTime;
        ui->logText->append(log);
        ui->logText->append("发送完毕");
    }
}
