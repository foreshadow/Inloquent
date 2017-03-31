#ifndef MODEL_H
#define MODEL_H

#include <QMap>

class Model : public QMap<QString, QString>
{
public:
    Model();
    virtual ~Model();
};

#include "inloquentmodel.h"

#endif // MODEL_H
