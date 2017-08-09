#ifndef DB_H
#define DB_H

#include <functional>

#include <QString>

#include "builder.h"
#include "blueprint.h"

class DB
{
    friend class Builder;
    friend class BluePrint;

    /**
     * Using reference type will allow users to
     * use BluePrint value type as the parameter,
     * which makes the function no effect, and
     * an table with no columns will be created.
     *
     * Pointer type also provides a php-like usage:
     *     table->integer("id");
     */
    using F = std::function<void(BluePrint *)>;

public:
    static bool initialize(const QString &driver, const QString &host, int port,
                           const QString &username, const QString &password,
                           const QString &database);

    static Builder table(const QString &table);

    static bool create(const QString &tableName, F func);
    static bool createIfNotExists(const QString &tableName, F func);

    static bool isSqlite();

    static QString lastErrorMessage();

    static QString sqlTime(int relativeSeconds = 0);

private:
    static void setErrorMessage(const QString &error = QString());

protected:
    static QString errorMessage;
    static QString driver;
};

#endif // DB_H
