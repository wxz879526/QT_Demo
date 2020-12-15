#include "DatabaseManager.h"
#include <QSqlDatabase>


DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager s;
    return s;
}

DatabaseManager::~DatabaseManager()
{
    mDatabase->close();
    delete mDatabase;
}

DatabaseManager::DatabaseManager(const QString &path)
    : mDatabase(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")))
    , albumDao(*mDatabase)
    , picDao(*mDatabase)
{
    mDatabase->setDatabaseName(path);
    mDatabase->open();

    albumDao.init();
    picDao.init();
}
