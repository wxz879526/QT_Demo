#ifndef PICTUREDAO_H
#define PICTUREDAO_H

#include <QVector>
#include <memory>
#include <vector>

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
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> pictures() const;

private:
    QSqlDatabase& mDatabase;
};

#endif // PICTUREDAO_H
