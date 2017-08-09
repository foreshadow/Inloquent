#include "model.h"

#include <QDebug>
#include "db.h"

Model::Model() :
    exists(false),
    data(),
    original()
{
}

Model::Model(const QMap<QString, QVariant> &map) :
    exists(false),
    data(map),
    original()
{
}

Model::~Model()
{
}

QString Model::get(const QString &key) const
{
    return data.value(key).toString();
}

int Model::getInt(const QString &key) const
{
    bool ok = true;
    int intValue = data.value(key).toInt(&ok);
    if (!ok)
        qDebug("Not a int value: trying to convert %s = `%s' to integer.",
               key.toLatin1().data(), data.value(key).toString().toLatin1().data());
    return intValue;
}

void Model::set(const QString &key, const QVariant &value)
{
    data[key] = value;
}

int Model::id() const
{
    return getInt("id");
}

QStringList Model::keys() const
{
    return data.keys();
}

QStringList Model::dirtyKeys() const
{
    QStringList dirty;
    for (QString key : data.keys()) {
        if (original.count(key) == 0 || data.value(key) != original.value(key))
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
    return data.size() || original.size();
}

Model::AttributeRef Model::operator [](const char *key)
{
    return AttributeRef(*this, key);
}

Model::AttributeRef Model::operator [](const QString &key)
{
    return AttributeRef(*this, key);
}

void Model::saved()
{
    exists = true;
    original = data;
}

Model::AttributeRef::AttributeRef(Model &model, const QString &key) :
    model(model),
    key(key)
{
}

QVariant &Model::AttributeRef::operator =(const QVariant &value) const
{
    model.set(key, value);
    return model.data[key];
}

QString Model::AttributeRef::toString() const
{
    return model.get(key);
}

Model::AttributeRef::operator QString() const
{
    return model.get(key);
}

Model::AttributeRef::operator int() const
{
    return model.getInt(key);
}

Model::AttributeRef::operator QVariant() const
{
    return model.data[key];
}
