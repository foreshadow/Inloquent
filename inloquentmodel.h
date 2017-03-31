#ifndef INLOQUENTMODEL_H
#define INLOQUENTMODEL_H

#include <QString>
#include "builder.h"
#include "collection.h"

#define BUILDER_ADAPTER(method) \
    template<typename ...types> \
    static Builder method(types... args) \
    { return Builder(table()).method(args...); }

#define INLOQUENT_MODEL(className, tableName, primaryKey) \
    public: \
        className(Model model) : Model(model) \
        {} \
        static QString table() \
        { return #tableName; } \
        static QString primaryKey() \
        { return #primaryKey; } \
        static Collection all() \
        { return Builder(table()).get(); } \
        static Model find(const QVariant &key) \
        { return Builder(table()).where(primaryKey(), key).first(); } \
        BUILDER_ADAPTER(where) \
    private:

#define INLOQUENT_MODEL_CLASS(className) \
    class className : public Model \
    { INLOQUENT_MODEL(className, className, id) };

#endif // INLOQUENTMODEL_H
