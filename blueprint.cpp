#include "blueprint.h"

#include <QStringList>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "db.h"

BluePrint::BluePrint(const QString &predicate, const QString &table) :
    predicate(predicate),
    table(table)
{
}

BluePrint::~BluePrint()
{
    for (Column *c : columns)
        delete c;
}

BluePrint::Column *BluePrint::increment(const QString &name)
{
    columns.append(new Column(name, DB::isSqlite() ? "integer" : "int"));
    return columns.back()->primaryKey()->autoIncrement();
}

BluePrint::Column *BluePrint::integer(const QString &name)
{
    columns.append(new Column(name, DB::isSqlite() ? "integer" : "int"));
    return columns.back();
}

BluePrint::Column *BluePrint::string(const QString &name)
{
    columns.append(new Column(name, "char(255)"));
    return columns.back();
}

void BluePrint::timestamps()
{
    columns.append(new Column("created_at", "timestamp"));
    columns.append(new Column("updated_at", "timestamp"));
}

bool BluePrint::commit()
{
    QStringList lines;
    for (Column *c : columns)
        lines.append(c->toSql());
    QString statement = QString("%1 %2 (%3)").arg(predicate, table, lines.join(", "));
    return Builder::exec(statement);
}

BluePrint::Column::Column(const QString &name, const QString &type, bool primaryKey) :
    name(name),
    type(type),
    pk(primaryKey)
{
}

BluePrint::Column *BluePrint::Column::primaryKey()
{
    pk = true;
    return this;
}

BluePrint::Column *BluePrint::Column::nullable()
{
    nn = false;
    return this;
}

BluePrint::Column *BluePrint::Column::unique()
{
    u = true;
    return this;
}

BluePrint::Column *BluePrint::Column::autoIncrement()
{
    ai = true;
    return this;
}

QString BluePrint::Column::toSql()
{
    QString line = QString("%1 %2").arg(name).arg(type);
    if (pk)
        line.append(" primary key");
    if (u)
        line.append(" unique");
    if (ai)
        line.append(DB::isSqlite() ? " autoincrement" : " auto_increment");
    if (nn)
        line.append(" not null");
    return line;
}
