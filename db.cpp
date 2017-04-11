#include "db.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

bool DB::initialize(const QString &driver, const QString &database,
                    const QString &username, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);
    if (db.open()) {
        qDebug("数据库连接成功。");
    } else {
        qDebug("数据库连接失败。");
        qDebug() << db.lastError().text();
    }
    return db.isOpen();
}

Builder DB::table(const QString &table)
{
    return Builder(table);
}
