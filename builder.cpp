#include "builder.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include "inloquent/db.h"

Builder::Builder(const QString &table) :
    tableClause(table),
    limit(0),
    offset(0)
{
}

bool Builder::insert(Model &model)
{
    if (model.useTimestamps()) {
        QString now = DB::sqlTime();
        model.set(model.created_at(), now);
        model.set(model.updated_at(), now);
    }
    QStringList columns, values;
    for (QString k : model.keys()) {
        columns.append(escapeKey(k));
        values.append(escapeValue(model[k]));
    }
    QString queryStatement = QString("insert into %1 (%2) values (%3)")
                             .arg(escapeTable()).arg(columns.join(", ")).arg(values.join(", "));
    QSqlQuery query;
    if (query.exec(queryStatement)) {
        // TODO: primarkey?
        model.set("id", query.lastInsertId());
        model.exists = true;
        return true;
    } else {
//        qDebug() << query.lastError().text();
        return false;
    }
}

bool Builder::update(Model &model)
{
    model.touch();
    if (model.dirtyKeys().empty())
        return true;
    QStringList assignments;
    for (QString k : model.dirtyKeys())
        assignments.append(QString("%1 = %2").arg(escapeKey(k)).arg(escapeValue(model[k])));
    QString queryStatement
        = QString("update %1 set %2").arg(escapeTable()).arg(assignments.join(", "));
    if (whereClause.size()) {
        queryStatement += QString(" where %1").arg(whereClause);
    } else {
        qDebug() << "No WHERE clause in update operation, stop.";
        return false;
    }
    QSqlQuery query;
    if (query.exec(queryStatement)) {
        model.original = model;
        return true;
    } else {
        qDebug() << query.lastError().text();
        return false;
    }
}

Builder &Builder::where(const QString &key, const QString &op, const QVariant &value, const QString &boolean)
{
    if (whereClause.size())
        whereClause += QString(" %1 ").arg(boolean);
    whereClause += QString("%1 %2 %3").arg(escapeKey(key)).arg(op).arg(escapeValue(value));
    return *this;
}

Builder &Builder::where(const QString &key, const QVariant &value)
{
    return where(key, "=", value);
}

Builder &Builder::take(int limit)
{
    this->limit = limit;
    return *this;
}

Builder &Builder::skip(int offset)
{
    this->offset = offset;
    return *this;
}

Collection Builder::get(const QString &column) const
{
    QString queryStatement = QString("select %1 from %2").arg(column).arg(tableClause);
    if (whereClause.size())
        queryStatement += QString(" where %1").arg(whereClause);
    if (groupByClause.size())
        queryStatement += QString(" group by %1").arg(groupByClause);
    if (havingClause.size())
        queryStatement += QString(" having %1").arg(havingClause);
    if (orderByClause.size())
        queryStatement += QString(" order by %1").arg(orderByClause);
    if (limit)
        queryStatement += QString(" limit %1, %2").arg(offset).arg(limit);
    QSqlQuery query;
    Collection collection;
    if (query.exec(queryStatement)) {
        QSqlRecord record = query.record();
        while (query.next()) {
            Model m;
            m.exists = true;
            for (int i = 0; i < record.count(); i++)
                m.insert(record.fieldName(i), query.value(i).toString());
            m.original = m;
            collection.append(m);
        }
    }
    return collection;
}

Model Builder::first()
{
    return take(1).get().first();
}

Model Builder::firstOrFail()
{
    Collection collection = take(1).get();
    return collection.size() ? collection.first() : Model::invalid();
}

QString Builder::escapeTable()
{
    return QString("`%1`").arg(tableClause);
}

QString Builder::escapeKey(const QString &key)
{
    return QString("`%1`.`%2`").arg(tableClause).arg(key);
}

QString Builder::escapeValue(const QVariant &value)
{
    switch (value.type()) {
    case QVariant::Bool:
        return QString::number(value.toBool() ? 1 : 0);
    case QVariant::Int:
        return QString::number(value.toInt());
    default:
        return QString("'%1'").arg(value.toString().replace("'", "\'"));
    }
}
