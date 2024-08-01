#include "mainwindow.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建SQLite数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/haikez/sqlite/testdb.db");

    // 打开数据库
    if (!db.open()) {
        qDebug() << "无法打开数据库!";
        return -1;
    }
    qDebug()<<"数据库打开完成";

    // 创建一个查询对象
    QSqlQuery query;

    // 执行查询
    if (!query.exec("SELECT * FROM test")) {
        qDebug() << "查询失败: ";
    } else {
        while (query.next()) {
            QString id = query.value("id").toString();
            QString name = query.value("name").toString();
            qDebug() << "从数据库读取的值: " << id<<" "<<name;
        }
    }

    // 关闭数据库连接
    db.close();

    // MainWindow w;
    // w.show();
    return a.exec();
}
