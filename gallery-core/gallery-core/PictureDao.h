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

    void addPictureInAlbum(int albumId, Picture &pic) const;
    void updataPicture(const Picture &pic) const;
    void removePicture(int id) const;
    void removePicturesForAlbum(int albumId) const;
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> picturesForAlbum(int albumId) const;

private:
    QSqlDatabase& mDatabase;
};

#endif // PICTUREDAO_H
