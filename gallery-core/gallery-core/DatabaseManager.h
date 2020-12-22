#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include "AlbumDao.h"
#include "PictureDao.h"

class QSqlDatabase;
class QSqlQuery;

const QString DATABASE_FILENAME = "gallery.db";

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    ~DatabaseManager();

    void debugQuery(const QSqlQuery& query);

protected:
    DatabaseManager(const QString &path = DATABASE_FILENAME);
    DatabaseManager &operator=(const DatabaseManager &rhs);

public:
    QSqlDatabase *mDatabase;
    const AlbumDao albumDao;
    const PictureDao picDao;
};

#endif // DATABASEMANAGER_H
