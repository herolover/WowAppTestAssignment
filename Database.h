#pragma once

#include "models/Account.h"
#include "models/Group.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMap>

class Database: public QObject
{
    Q_OBJECT

public:
    Database(const QString &dbName, QObject *parent = nullptr);

    GroupId addGroup(const Group &group);
    void addAccount(const Account &account);

    QSqlTableModel *_accountsModel;
private:
    void loadGroupCache();
    void createDatabaseScheme();
    void createPreparedStatements();

    QSqlDatabase _db;
    QSqlQuery *_insertGroupQuery;
    QSqlQuery *_insertAccountQuery;

    QMap<QString, GroupId> _groupCache;
};
