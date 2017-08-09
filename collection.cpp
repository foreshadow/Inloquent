#include "collection.h"

Collection::operator bool() const
{
    return !!size();
}
