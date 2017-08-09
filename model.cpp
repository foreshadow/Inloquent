#include "model.h"

#include <QDebug>
#include "inloquent/db.h"

Model::Model() :
    QMap<QString, QVariant>(),
    exists(false),
    valid(true)
{
}

Model::Model(const QMap<QString, QVariant> &map) :
    QMap<QString, QVariant>(map),
    exists(false),
    valid(true)
{
}

Model::~Model()
{
}

QString Model::get(const QString &key) const
{
    return value(key).toString();
}

int Model::getInt(const QString &key) const
{
    bool ok = true;
    int intValue = value(key).toInt(&ok);
    if (!ok)
        qDebug("Not a int value: trying to convert %s = %s to integer.",
               key.toLatin1().data(), value(key).toString().toLatin1().data());
    return intValue;
}

void Model::set(const QString &key, const QVariant &value)
{
    (*this)[key] = value;
}

int Model::id() const
{
    return getInt("id");
}

QStringList Model::dirtyKeys() const
{
    QStringList dirty;
    for (QString key : keys()) {
        if (original.count(key) == 0 || value(key) != original.value(key))
            dirty.append(key);
    }
    return dirty;
}

QString Model::created_at() const
{
    return "created_at";
}

QString Model::updated_at() const
{
    return "updated_at";
}

bool Model::useTimestamps() const
{
    return true;
}

void Model::touch()
{
    if (useTimestamps())
        set(updated_at(), DB::sqlTime());
}

Model::operator bool() const
{
    return valid;
}

Model Model::invalid()
{
    return Model(Flag::INVALID);
}

Model::Model(Model::Flag) :
    valid(false)
{
}
