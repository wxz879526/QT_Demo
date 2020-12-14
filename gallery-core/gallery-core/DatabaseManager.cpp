#include "DatabaseManager.h"
#include <QSqlDatabase>


DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager s;
    return s;
}

DatabaseManager::~DatabaseManager()
{

}

DatabaseManager::DatabaseManager(const QString &path)
    : mDatabase(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")))
{
    mDatabase->setDatabaseName(path);
    mDatabase->open();
}

DatabaseManager& DatabaseManager::operator=(const DatabaseManager &rhs)
{
    mDatabase->close();
    delete mDatabase;
}
