#ifndef GALLERYCORE_H
#define GALLERYCORE_H

#include <QString>
#include "gallery-core_global.h"

class GALLERYCORE_EXPORT Album
{
public:
    explicit Album(const QString &name = "");

    int id() const;
    void setId(int id);
    QString name();
    void setName(const QString &name);

private:
    int mId;
    QString mName;
};

#endif // GALLERYCORE_H
