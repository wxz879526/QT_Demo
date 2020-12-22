#include "DatabaseManager.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
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

void DatabaseManager::debugQuery(const QSqlQuery &query)
{
    if (query.lastError().type() == QSqlError::ErrorType::NoError)
    {
         qDebug() << "Query OK:" << query.lastQuery();
    }
    else
    {
        qWarning() <<"Query KO:" << query.lastError().text();
        qWarning() << "Query text:" << query.lastQuery();
    }
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
