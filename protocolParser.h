#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H
#include <QDomDocument>
#include <QtTypes>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QList>
#include <QVariant>

#pragma pack(push, 1)
// 定义一个结构体存储字段信息
typedef struct Field{
    QString fieldName;
    QString isSelected;
    QString datatype;
    QVariant data;
    quint32 bitIndex;
    quint32 length;
    quint32 loopEnd;
    quint32 minimum;
    quint32 maximum;
    quint32 precision;
    QString isKey;
}Field;
typedef struct Protocol{
    QString sourceIP;
    uint sourcePort;
    QString destIP;
    uint destPort;
    QList<Field> fields;
} Protocol;

#pragma pack(pop)

class ProtocolParser {
public:
    bool parseXML(const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file:" << filePath;
            return false;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            qDebug() << "Failed to parse XML file.";
            file.close();
            return false;
        }
        file.close();

        QDomElement root = doc.documentElement();
        // 解析XML内容
        if (root.tagName() != "protocol") {
            qDebug() << "Invalid root element:" << root.tagName();
            return false;
        }
        // 解析字段信息
        protocol.sourceIP=root.elementsByTagName("sourceIP").at(0).toElement().text();
        protocol.destIP=root.elementsByTagName("destIP").at(0).toElement().text();
        protocol.sourcePort=root.elementsByTagName("sourcePort").at(0).toElement().text().toUInt();
        protocol.destPort=root.elementsByTagName("destPort").at(0).toElement().text().toUInt();
        QDomNodeList fieldNodes = root.elementsByTagName("field");

        for (int i = 0; i < fieldNodes.count(); ++i) {
            QDomElement fieldElement = fieldNodes.at(i).toElement();
            Field field;
            field.fieldName = fieldElement.elementsByTagName("fieldName").at(0).toElement().text();
            field.isSelected = fieldElement.elementsByTagName("isSelected").at(0).toElement().text();
            field.datatype = fieldElement.elementsByTagName("datatype").at(0).toElement().text();
            field.bitIndex = fieldElement.elementsByTagName("bitIndex").at(0).toElement().text().toUInt();
            field.length = fieldElement.elementsByTagName("length").at(0).toElement().text().toUInt();
            field.loopEnd = fieldElement.elementsByTagName("loopEnd").at(0).toElement().text().toUInt();
            field.minimum = fieldElement.elementsByTagName("minimum").at(0).toElement().text().toUInt(nullptr,16);
            field.maximum = fieldElement.elementsByTagName("maximum").at(0).toElement().text().toUInt(nullptr,16);
            field.precision = fieldElement.elementsByTagName("precision").at(0).toElement().text().toUInt(nullptr,16);
            field.isKey = fieldElement.elementsByTagName("isKey").at(0).toElement().text();

            // 根据字段类型解析数据
            if (field.datatype == "DEC") {
                field.data = fieldElement.elementsByTagName("data").at(0).toElement().text().toUInt();
            } else if (field.datatype == "INT") {
                field.data = fieldElement.elementsByTagName("data").at(0).toElement().text().toInt();
            } else if (field.datatype == "BIN") {
                // 二进制数据解析
                QByteArray binaryData = QByteArray::fromHex(fieldElement.elementsByTagName("data").at(0).toElement().text().toLatin1());
                field.data = QVariant::fromValue(binaryData);
            } else if (field.datatype == "OCT") {
                // 八进制数据解析
                field.data = fieldElement.elementsByTagName("data").at(0).toElement().text().toUInt(nullptr, 8);
            } else if (field.datatype == "HEX") {
                field.data = fieldElement.elementsByTagName("data").at(0).toElement().text().toUInt(nullptr, 16);
            } else if (field.datatype == "FLT") {
                field.data = fieldElement.elementsByTagName("data").at(0).toElement().text().toFloat();
            } else if (field.datatype == "DBL") {
                field.data = fieldElement.elementsByTagName("data").at(0).toElement().text().toDouble();
            } else if (field.datatype == "STRING") {
                field.data = fieldElement.elementsByTagName("data").at(0).toElement().text();
            } else if (field.datatype == "FLAG") {
                field.data = fieldElement.elementsByTagName("data").at(0).toElement().text().toUInt();
            }

            fields.append(field);
        }
        protocol.fields = fields;
        return true;
    }
    Protocol getProtocol(const QString& filePath){
        parseXML(filePath);
        return this->protocol;
    }
private:
    QList<Field> fields;
    Protocol protocol;
    Field field;
};

#endif // PROTOCOLPARSER_H
