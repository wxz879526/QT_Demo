#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include "Album.h"
#include <QVariant>
#include "AlbumDao.h"

#include "DatabaseManager.h"

AlbumDao::AlbumDao(QSqlDatabase& database)
	: mDatabase(database)
{

}

void AlbumDao::init() const
{
	if (!mDatabase.tables().contains("albums"))
	{
		QSqlQuery query(mDatabase);
		query.exec("CREATE TABLE albums (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
    }
}

void AlbumDao::addAlbum(Album &album) const
{
    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO albums (name) VALUES(:name)");
    query.bindValue(":name", album.name());
    query.exec();
    album.setId(query.lastInsertId().toInt());
}

void AlbumDao::updataAlbum(const Album &album) const
{
    QSqlQuery query(mDatabase);
    query.prepare("UPDATE albums SET name=:name WHERE id=:id");
    query.bindValue(":name", album.name());
    query.bindValue(":id", album.id());
    query.exec();
}

void AlbumDao::removeAlbum(int id) const
{
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM albums WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();
}

std::unique_ptr<std::vector<std::unique_ptr<Album>>> AlbumDao::albums() const
{
    QSqlQuery query("SELECT * FROM albums", mDatabase);
    query.exec();
    std::unique_ptr<std::vector<std::unique_ptr<Album>>> list;
    while (query.next())
    {
        std::unique_ptr<Album> album = std::make_unique<Album>();
        album->setId(query.value("id").toInt());
        album->setName(query.value("name").toString());
        list->push_back(std::move(album));
    }

    return std::move(list);
}
