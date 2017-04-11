#ifndef BUILDER_H
#define BUILDER_H

#include <QString>
#include <QVariant>
#include "model.h"
#include "collection.h"

class Collection;

class Builder
{
public:
    Builder(const QString &table);

    bool insert(Model &model);
    bool update(Model &model);

    Builder &where(const QString &key, const QString &op, const QVariant &value);
    Builder &where(const QString &key, const QVariant &value);

    Builder &take(int count);
    Collection get(const QString &column = "*") const;
    Model first();
    Model firstOrFail();

private:
    static QString escapeKey(const QString &key);
    static QString escapeValue(QVariant value);

protected:
    QString tableClause;
    QString whereClause;
    QString groupByClause;
    QString havingClause;
    QString orderByClause;
    int count;
    int offset;
};

#endif // BUILDER_H
