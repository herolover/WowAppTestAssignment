#include "Database.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>

Database::Database(const QString &dbName, QObject *parent)
    : QObject(parent)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    QDir dir(QCoreApplication::applicationDirPath());
    _db.setDatabaseName(dir.absoluteFilePath(dbName));
    _db.open();

    if (_db.tables().isEmpty()) {
        createDatabaseScheme();
    }
}

Database::AddGroupAccountGuard Database::makeAddGroupAccountGuard()
{
    return AddGroupAccountGuard(*this, _db);
}

QSqlQueryModel *Database::createGroupListModel()
{
    auto model = new QSqlQueryModel();
    QSqlQuery selectGroups(_db);
    selectGroups.exec(R"(
        SELECT `group_id`, `group_name`
        FROM `groups`
        ORDER BY `group_order`
    )");
    selectGroups.exec();
    model->setQuery(selectGroups);

    return model;
}

AccountListModel *Database::createAccountListModel(GroupId groupId)
{
    QSqlQuery selectAccounts(_db);
    selectAccounts.prepare(R"(
        SELECT `id`, `first_name`, `last_name`, `sex`
        FROM `accounts`
        WHERE `group_id` = :group_id
        AND
        (
            `first_name` LIKE :like_filter
            OR `last_name` LIKE :like_filter
        )
        ORDER BY `first_name` ASC, `last_name` ASC
    )");
    selectAccounts.bindValue(":group_id", groupId);
    selectAccounts.bindValue(":like_filter", "%");
    selectAccounts.exec();

    auto model = new AccountListModel();
    model->setQuery(selectAccounts);

    connect(model, &AccountListModel::likeFilterChanged, [model](QString likeFilter) {
        auto query = model->query();
        query.bindValue(":like_filter", "%" + likeFilter + "%");
        query.exec();

        model->setQuery(query);
    });

    return model;
}

QVector<AccountListModel *> Database::createAccountListModelList(QSqlQueryModel *groupModel)
{
    QVector<AccountListModel *> modelList;
    for (int i = 0; i < groupModel->rowCount(); ++i) {
        GroupId groupId = groupModel->record(i).value(0).value<GroupId>();
        modelList.append(createAccountListModel(groupId));
    }

    return modelList;
}

void Database::createDatabaseScheme()
{
    _db.exec(R"(
        CREATE TABLE `groups` (
            `group_id` INTEGER PRIMARY KEY,
            `group_name` TEXT UNIQUE,
            `group_order` INTEGER NOT NULL
        )
    )");

    _db.exec(R"(
        CREATE TABLE `accounts` (
            `id` TEXT PRIMARY KEY,
            `username` TEXT NOT NULL,
            `first_name` TEXT NOT NULL,
            `last_name` TEXT NOT NULL,
            `sex` INTEGER NOT NULL,
            `country` TEXT NULL,
            `language` TEXT NULL,
            `birthday` INTEGER NOT NULL,
            `group_id` INTEGER NOT NULL,
            `roster_id` TEXT NOT NULL,

            FOREIGN KEY (`group_id`) REFERENCES `groups`(`group_id`)
        )
    )");

    // in our case where there are only 3 groups that index is inefficient
    // so it's better to remove it - we save disk space and improve the insertion performance
    _db.exec(R"(
        CREATE INDEX `accounts_sort_index` ON `accounts`(`group_id`, `first_name`, `last_name`)
    )");
}

Database::AddGroupAccountGuard::AddGroupAccountGuard(Database &parent, QSqlDatabase &db)
    : _parent(parent)
    , _db(db)
{
    _insertGroupQuery = new QSqlQuery(_db);
    _insertGroupQuery->prepare(R"(
        INSERT OR IGNORE INTO `groups`(`group_name`, `group_order`)
        VALUES (?, ?)
    )");

    _insertAccountQuery = new QSqlQuery(_db);
    _insertAccountQuery->prepare(R"(
        INSERT INTO `accounts`
        (
            `id`, `username`, `first_name`, `last_name`, `sex`, `country`, `language`, `birthday`, `group_id`, `roster_id`
        )
        VALUES
        (
            :id, :username, :first_name, :last_name, :sex, :country, :language, :birthday, :group_id, :roster_id
        )
    )");

    _db.transaction();
}

Database::AddGroupAccountGuard::~AddGroupAccountGuard()
{
    _db.commit();
    emit _parent.groupAccountUpdated();
}

GroupId Database::AddGroupAccountGuard::addGroup(const Group &group)
{
    auto groupIt = _groupCache.find(group.groupName);
    if (groupIt == _groupCache.end()) {
        _insertGroupQuery->bindValue(0, group.groupName);
        _insertGroupQuery->bindValue(1, group.groupOrder);
        _insertGroupQuery->exec();

        auto groupId = _insertGroupQuery->lastInsertId().value<GroupId>();
        _groupCache[group.groupName] = groupId;

        return groupId;
    } else {
        return groupIt.value();
    }
}

bool Database::AddGroupAccountGuard::addAccount(const Account &account)
{
    _insertAccountQuery->bindValue(":id", account.id);
    _insertAccountQuery->bindValue(":username", account.username);
    _insertAccountQuery->bindValue(":first_name", account.firstName);
    _insertAccountQuery->bindValue(":last_name", account.lastName);
    _insertAccountQuery->bindValue(":sex", static_cast<int>(account.sex));
    _insertAccountQuery->bindValue(":country", account.country);
    _insertAccountQuery->bindValue(":language", account.language);
    _insertAccountQuery->bindValue(":birthday", account.birthday);
    _insertAccountQuery->bindValue(":group_id", account.groupId);
    _insertAccountQuery->bindValue(":roster_id", account.rosterId);
    if (!_insertAccountQuery->exec()) {
        qCritical() << _insertAccountQuery->lastError().text();

        return false;
    } else {
        return true;
    }
}

void Database::AddGroupAccountGuard::loadGroupCache()
{
    QSqlQuery selectGroups(_db);
    selectGroups.exec(R"(
        SELECT `group_id`, `group_name`
        FROM `groups`
    )");
    while (selectGroups.next()) {
        auto groupId = selectGroups.value("group_id").value<GroupId>();
        auto groupName = selectGroups.value("group_name").value<QString>();
        _groupCache[groupName] = groupId;
    }
}
