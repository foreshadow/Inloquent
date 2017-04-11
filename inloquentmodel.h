#ifndef INLOQUENTMODEL_H
#define INLOQUENTMODEL_H

#include <QString>
#include "builder.h"
#include "collection.h"

#define BUILDER_ADAPTER(method) \
    template<typename ...types> \
    static Builder method(types... args) \
    { return Builder(table()).method(args...); }

#define INLOQUENT_MODEL(class_, tableName, primaryKey_) \
    public: \
        class_() \
        {} \
        class_(Model model) : Model(model) \
        {} \
        static QString className() \
        { return #class_; } \
        static QString table() \
        { return #tableName; } \
        static QString primaryKey() \
        { return #primaryKey_; } \
        static Collection all() \
        { return Builder(table()).get(); } \
        static Model find(const QVariant &key) \
        { return Builder(table()).where(primaryKey(), key).first(); } \
        bool save() \
        { return exists ? Builder(table()).where(primaryKey(), get(primaryKey())).update(*this) : Builder(table()).insert(*this); } \
        /* relations */ \
        template<class T> Collection hasMany(const QString &foreignKey = className().toLower() + "_" + primaryKey(), const QString &localKey = primaryKey()) const \
        { return Builder(T::table()).where(foreignKey, get(localKey)).get(); } \
        template<class T> T hasOne(const QString &foreignKey = className().toLower() + "_" + primaryKey(), const QString &localKey = primaryKey()) const \
        { return Builder(T::table()).where(foreignKey, get(localKey)).firstOrFail(); } \
        template<class T> Collection belongsToMany(const QString &foreignKey = className().toLower() + "_" + primaryKey(), const QString &localKey = primaryKey()) const \
        { return Builder(T::table()).where(localKey, get(foreignKey)).get(); } \
        template<class T> T belongsTo(const QString &foreignKey = className().toLower() + "_" + primaryKey(), const QString &localKey = primaryKey()) const \
        { return Builder(T::table()).where(localKey, get(foreignKey)).firstOrFail(); } \
        BUILDER_ADAPTER(where) \
    private:

#define INLOQUENT_MODEL_CLASS(class_) \
    class class_ : public Model \
    { INLOQUENT_MODEL(class_, class_, id) };

#endif // INLOQUENTMODEL_H
