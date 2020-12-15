#ifndef PICTUREDAO_H
#define PICTUREDAO_H

#include <QVector>

class QSqlDatabase;
class Picture;

class PictureDao
{
public:
    PictureDao(QSqlDatabase &database);
    void init() const;

    void addPicture(Picture &pic) const;
    void updataPicture(const Picture &pic) const;
    void removePicture(int id) const;
    QVector<Picture *> pictures() const;

private:
    QSqlDatabase& mDatabase;
};

#endif // PICTUREDAO_H
