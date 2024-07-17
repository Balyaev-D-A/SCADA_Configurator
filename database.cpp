#include <QStringList>
#include <QMessageBox>
#include <QVariant>
#include "database.h"

Database::Database()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    pdb = &db;
    pq = new QSqlQuery(db);
}

Database::~Database()
{
    delete pq;
}

bool Database::open(QString dbhost, QString dbname, QString dbuser, QString dbpwd)
{
    pdb->setHostName(dbhost);
    pdb->setDatabaseName(dbname);
    pdb->setUserName(dbuser);
    pdb->setPassword(dbpwd);
    return pdb->open();
}

QSqlError Database::lastError()
{
    return pdb->lastError();
}

QString Database::explodeFields(QStringList fields, unsigned char from)
{
    QString result = "";
    for (int i=from; i<fields.count(); i++ ) {
        if (i>from) result += ", ";
        result += fields.at(i);
    }
    return result;
}

bool Database::deployTables()
{
    return true;
}

void Database::showError(QWidget *sender)
{
    QMessageBox::critical(sender, "Ошибка выполнения запроса!", pq->lastError().text());
}

bool Database::execQuery(const QString &query)
{
    return pq->exec(query);
}

QVariant Database::fetchValue(int index)
{
    return pq->value(index);
}

QVariant Database::fetchValue(const QString &name)
{
    return pq->value(name);
}

int Database::affectedRows() const
{
    return pq->numRowsAffected();
}

bool Database::nextRecord()
{
    return pq->next();
}

bool Database::startTransaction()
{
    return pdb->transaction();
}

bool Database::commitTransaction()
{
    return pdb->commit();
}

bool Database::rollbackTransaction()
{
    return pdb->rollback();
}

QVariant Database::lastInsertId() const
{
    return pq->lastInsertId();
}

bool Database::seekRecord(int index, bool relative)
{
    return pq->seek(index, relative);
}

void Database::close()
{
    pdb->close();
}

bool Database::firstRecord()
{
    return pq->first();
}

QVariant Database::getVariable(QString varName)
{
    QVariant result;
    QString query = "SELECT var_value FROM variables WHERE var_name = '%1'";
    query = query.arg(varName);
    if (!execQuery(query))
        return result;
    if (nextRecord()) {
        result = fetchValue(0);
    }
    return result;
}

QList<QStringList> Database::getResults()
{
    QStringList resultRow;
    QList<QStringList> result;

    while (nextRecord())
    {
        resultRow.clear();
        for (int i=0; i<pq->record().count(); i++)
            resultRow.append(fetchValue(i).toString());
        result.append(resultRow);
    }
    return result;
}
