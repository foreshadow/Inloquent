#include "model.h"

#include <QDebug>

Model::Model() :
    exists(false), valid(true)
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
        qDebug() << "Not a int value";
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
