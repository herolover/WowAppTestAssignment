#pragma once

#include "models/Account.h"
#include "models/AccountListModel.h"
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

    class AddGroupAccountGuard
    {
    public:
        AddGroupAccountGuard(Database &parent, QSqlDatabase &db);
        AddGroupAccountGuard(AddGroupAccountGuard &&) = default;
        ~AddGroupAccountGuard();

        GroupId addGroup(const Group &group);
        bool addAccount(const Account &account);

    private:
        void loadGroupCache();

        Database &_parent;
        QSqlDatabase &_db;
        QSqlQuery *_insertGroupQuery;
        QSqlQuery *_insertAccountQuery;

        QMap<QString, GroupId> _groupCache;
    };

    AddGroupAccountGuard makeAddGroupAccountGuard();

    QSqlQueryModel *createGroupListModel();
    AccountListModel *createAccountListModel(GroupId groupId);
    QVector<AccountListModel *> createAccountListModelList(QSqlQueryModel *groupModel);

signals:
    void groupAccountUpdated();

private:
    void createDatabaseScheme();

    QSqlDatabase _db;
};
