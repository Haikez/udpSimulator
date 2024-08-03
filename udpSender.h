#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <QUdpSocket>
#include <QHostAddress>
#include "protocolParser.h"
#include <QTime>

class UdpSender : public QObject {
    Q_OBJECT

public:
    UdpSender(QObject *parent = nullptr) : QObject(parent), udpSocket(new QUdpSocket(this)) {}

    // 序列化函数
    QByteArray serializeProtocol(const Protocol& protocol) {
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian); // 设置字节序

        // // 序列化基本信息
        // stream << protocol.sourceIP;
        // stream << protocol.sourcePort;
        // stream << protocol.destIP;
        // stream << protocol.destPort;

        // 序列化字段
        for (const Field& field : protocol.fields) {
            stream << field.fieldName;
            stream << field.isSelected;
            stream << field.datatype;
            stream << field.data;
            stream << field.bitIndex;
            stream << field.length;
            stream << field.loopEnd;
            stream << field.minimum;
            stream << field.maximum;
            stream << field.precision;
            stream << field.isKey;
        }
        return byteArray;
    }

    void sending(const QByteArray &data, const QHostAddress &address, quint16 port) {
        udpSocket->writeDatagram(data, address, port);
    }

private:
    QUdpSocket *udpSocket;

};

#endif // UDPSENDER_H
