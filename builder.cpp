#include "builder.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

Builder::Builder(const QString &table) :
    tableClause(table),
    count(1),
    offset(0)
{

}

Builder &Builder::where(const QString &key, const QString &op, const QVariant &value)
{
    whereClause = QString("%1 %2 %3").arg(escapeKey(key)).arg(op).arg(escapeValue(value));
    return *this;
}

Builder &Builder::where(const QString &key, const QVariant &value)
{
    return where(key, "=", value);
}

Collection Builder::get(const QString &column) const
{
    QString queryStatement = QString("select %1 from %2").arg(column).arg(tableClause);
    if (whereClause.size()) {
        queryStatement += QString(" where %1").arg(whereClause);
    }
    if (groupByClause.size()) {
        queryStatement += QString(" group by %1").arg(groupByClause);
    }
    if (havingClause.size()) {
        queryStatement += QString(" having %1").arg(havingClause);
    }
    if (orderByClause.size()) {
        queryStatement += QString(" order by %1").arg(orderByClause);
    }
    queryStatement += QString(" limit %1, %2").arg(offset).arg(count);
    qDebug() << queryStatement;
    QSqlQuery query(queryStatement);
    Collection collection;
    if (query.exec()) {
        QSqlRecord record = query.record();
        while (query.next()) {
            collection.append(Model());
            for (int i = 0; i < record.count(); i++) {
                collection.last().insert(record.fieldName(i), query.value(i).toString());
            }
        }
    }
    return collection;
}

Model Builder::first() const
{
    Collection collection = get();
    if (collection.size()) {
        return get().first();
    } else {
        throw "Entry not found";
    }
}

QString Builder::escapeKey(const QString &key)
{
    return QString("`%1`").arg(key);
}

QString Builder::escapeValue(QVariant value)
{
    if (value.type() != QVariant::Int) {
        return QString("'%1'").arg(value.toString().replace("'", "\'"));
    } else {
        return QString::number(value.toInt());
    }
}
