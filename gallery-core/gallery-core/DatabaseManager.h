#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include "AlbumDao.h"
#include "PictureDao.h"

class QSqlDatabase;

const QString DATABASE_FILENAME = "gallery.db";

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    ~DatabaseManager();

protected:
    DatabaseManager(const QString &path = DATABASE_FILENAME);
    DatabaseManager &operator=(const DatabaseManager &rhs);

private:
    QSqlDatabase *mDatabase;
    const AlbumDao albumDao;
    const PictureDao picDao;
};

#endif // DATABASEMANAGER_H
