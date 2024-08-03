#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager {
public:
    DatabaseManager() {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("/Users/haikez/sqlite/log.db");
        // Open database
        if (!db.open()) {
            qDebug() << "Failed to open database:" << db.lastError().text();
        }
    }

    bool insertLog(const QString& sourceIP,uint sourcePort,const QString& destIP,uint destPort,
                   int frequency, int totalPackets,const QString &datatype,
                   const QString& startTime, const QString& endTime) {
        QSqlQuery query;
        query.prepare("INSERT INTO logs (sourceIP, sourcePort,destIP,destPort, frequency, totalPackets, datatype, startTime, endTime) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(sourceIP);
        query.addBindValue(sourcePort);
        query.addBindValue(destIP);
        query.addBindValue(destPort);
        query.addBindValue(frequency);
        query.addBindValue(totalPackets);
        query.addBindValue(datatype);
        query.addBindValue(startTime);
        query.addBindValue(endTime);
        return query.exec();
    }

private:
    QSqlDatabase db;
};
#endif // DATABASEMANAGER_H
