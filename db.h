#ifndef DB_H
#define DB_H

#include <QString>

#include "builder.h"

class DB
{
public:
    static bool initialize(const QString &driver, const QString &database,
                           const QString &username, const QString &password);
    static Builder table(const QString &table);
    static QString lastErrorMessage();

    static QString sqlTime(int relativeSeconds = 0);

protected:
    static QString errorMessage;
};

#endif // DB_H
