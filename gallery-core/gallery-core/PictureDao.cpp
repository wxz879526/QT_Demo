#include "PictureDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include "Picture.h"

PictureDao::PictureDao(QSqlDatabase &database)
    : mDatabase(database)
{

}

void PictureDao::init() const
{
    if (!mDatabase.tables().contains("pictures"))
    {
        QSqlQuery query(mDatabase);
        query.exec(QString("CREATE TABLE pictures (id INTEGER PRIMARY KEY AUTOINCREMENT, album_id INTEGER, url TEXT)"));
    }
}

void PictureDao::addPictureInAlbum(int albumId, Picture &pic) const
{
    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO pictures (album_id, url) VALUES(:album_id,:url)");
    query.bindValue(":album_id", albumId);
    query.bindValue(":url", pic.fileUrl());
    query.exec();
    pic.setId(query.lastInsertId().toInt());
    pic.setAlbumId(albumId);
}

void PictureDao::updataPicture(const Picture &pic) const
{
    QSqlQuery query(mDatabase);
    query.prepare("UPDATE pictures SET album_id=:album_id, url=:url WHERE id=:id");
    query.bindValue(":album_id", pic.albumId());
     query.bindValue(":url", pic.fileUrl());
    query.bindValue(":id", pic.id());
    query.exec();
}

void PictureDao::removePicture(int id) const
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM pictures WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();
}

void PictureDao::removePicturesForAlbum(int albumId) const
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM pictures WHERE album_id=:albumId");
    query.bindValue(":albumId", albumId);
    query.exec();
}

std::unique_ptr<std::vector<std::unique_ptr<Picture>>> PictureDao::picturesForAlbum(int albumId) const
{
    QSqlQuery query(mDatabase);
    query.prepare("SELECT * FROM pictures WHERE album_id=:albumId");
    query.bindValue(":albumId", albumId);
    query.exec();
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> list = std::make_unique<std::vector<std::unique_ptr<Picture>>>();
    while (query.next())
    {
        std::unique_ptr<Picture> pic = std::make_unique<Picture>();
        pic->setId(query.value("id").toInt());
        pic->setAlbumId(query.value("album_id").toInt());
        pic->setFileUrl(query.value("url").toString());
        list->push_back(std::move(pic));
    }

    return list;
}
