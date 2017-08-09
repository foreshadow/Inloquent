#include "db.h"

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

bool DB::initialize(const QString &driver, const QString &host, int port,
                    const QString &database, const QString &username, const QString &password)
{
    DB::driver = driver;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
    db.setHostName(host);
    db.setPort(port);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(database);
    if (db.open() == false)
        setErrorMessage(db.lastError().text());
    return db.isOpen();
}

Builder DB::table(const QString &table)
{
    return Builder(table);
}

bool DB::create(const QString &tableName, F func)
{
    BluePrint table("create table", tableName);
    func(&table);
    return table.commit();
}

bool DB::createIfNotExists(const QString &tableName, F func)
{
    BluePrint table("create table if not exists", tableName);
    func(&table);
    return table.commit();
}

bool DB::isSqlite()
{
    return driver.contains("SQLITE", Qt::CaseInsensitive);
}

QString DB::lastErrorMessage()
{
    return errorMessage;
}

QString DB::sqlTime(int relativeSeconds)
{
    return QDateTime::currentDateTime().addSecs(relativeSeconds).toString("yyyy-MM-dd HH:mm:ss");
}

void DB::setErrorMessage(const QString &error)
{
    errorMessage = error;
}

QString DB::errorMessage;
QString DB::driver("undefined");
