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

void PictureDao::addPicture(Picture &pic) const
{
    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO pictures (album_id, url) VALUES(:album_id,:url)");
    query.bindValue(":album_id", pic.albumId());
    query.bindValue(":url", pic.fileUrl());
    query.exec();
    pic.setId(query.lastInsertId().toInt());
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

QVector<Picture *> PictureDao::pictures() const
{
    QSqlQuery query("SELECT * FROM pictures", mDatabase);
    query.exec();
    QVector<Picture*> list;
    while (query.next())
    {
        Picture *pic = new Picture();
        pic->setId(query.value("id").toInt());
        pic->setAlbumId(query.value("album_id").toInt());
        pic->setFileUrl(query.value("url").toString());
        list.append(pic);
    }

    return list;
}
