#include "builder.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QTime>
#include <QTimer>
#include "db.h"

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
    if (Query query = exec(queryStatement)) {
        // TODO: if there is a key "id", it must be int + pk
        model.set("id", query.lastInsertId());
        model.saved();
        return true;
    } else {
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
    if (Query query = exec(queryStatement)) {
        model.saved();
        return true;
    } else {
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
    if (offset)
        queryStatement += QString(" limit %1, %2").arg(offset).arg(limit);
    else if (limit)
        queryStatement += QString(" limit %1").arg(limit);
    Collection collection;
    if (Query query = exec(queryStatement)) {
        QSqlRecord record = query.record();
        while (query.next()) {
            Model m;
            for (int i = 0; i < record.count(); i++)
                m.set(record.fieldName(i), query.value(i).toString());
            m.saved();
            collection.append(m);
        }
    }
    return collection;
}

Model Builder::first()
{
    Collection collection = take(1).get();
    return collection.size() ? collection.first() : Model();
}

Builder::Query Builder::exec(const QString &statement)
{
    return Query(statement);
}

QString Builder::escapeTable() const
{
    return QString("`%1`").arg(tableClause);
}

QString Builder::escapeKey(const QString &key) const
{
    return QString("`%1`.`%2`").arg(tableClause).arg(key);
}

QString Builder::escapeValue(const QVariant &value) const
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


Builder::Query::Query(const QString &statement) :
    QSqlQuery(),
    succeeded(false)
{
    QTime t;
    t.start();
    if ((succeeded = exec(statement)) == false) {
        DB::setErrorMessage(lastError().text());
    }
    qDebug(QString("[SqlQuery] %1 ms, %2").arg(t.elapsed(), 2).arg(statement).toLocal8Bit());
}

Builder::Query::operator bool()
{
    return succeeded;
}
