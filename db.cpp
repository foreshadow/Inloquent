#include "db.h"

#include <QDateTime>
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
    if (db.open() == false) {
        errorMessage = db.lastError().text();
    }
    return db.isOpen();
}

Builder DB::table(const QString &table)
{
    return Builder(table);
}

QString DB::lastErrorMessage()
{
    return errorMessage;
}

QString DB::sqlTime(int relativeSeconds)
{
    return QDateTime::currentDateTime().addSecs(relativeSeconds).toString("yyyy-MM-dd HH:mm:ss");
}

QString DB::errorMessage;
