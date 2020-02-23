#pragma once

#include "models/Account.h"
#include "models/Group.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMap>

class Database: public QObject
{
    Q_OBJECT

public:
    Database(const QString &dbName, QObject *parent = nullptr);

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    GroupId addGroup(const Group &group);
    bool addAccount(const Account &account);

    QSqlQueryModel *_groupTable;
    QSqlTableModel *_accountsModel;

    QSqlQueryModel *createAccountModel(GroupId groupId);
    QVector<QAbstractItemModel *> createAccountModelList();

private:
    void loadGroupCache();
    void createDatabaseScheme();
    void createPreparedStatements();

    QSqlDatabase _db;
    QSqlQuery *_insertGroupQuery;
    QSqlQuery *_insertAccountQuery;

    QMap<QString, GroupId> _groupCache;
};
