#include "Database.h"

#include <QDebug>
#include <QStringList>
#include <QSqlError>
#include <QDir>
#include <QCoreApplication>

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

    createPreparedStatements();
    loadGroupCache();

    _accountsModel = new QSqlTableModel(this, _db);
    _accountsModel->setTable("accounts");
    _accountsModel->select();
}

GroupId Database::addGroup(const Group &group)
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

void Database::addAccount(const Account &account)
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
    _insertAccountQuery->bindValue(":roaster_id", account.roasterId);
    if (!_insertAccountQuery->exec()) {
        qCritical() << _insertAccountQuery->lastError().text();
    }
}

void Database::loadGroupCache()
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
            `sex` INTEGER NULL,
            `country` TEXT NOT NULL,
            `language` TEXT NOT NULL,
            `birthday` INTEGER NOT NULL,
            `group_id` INTEGER NOT NULL,
            `roaster_id` TEXT NOT NULL,

            FOREIGN KEY (`group_id`) REFERENCES `groups`(`group_id`)
        )
    )");

    _db.exec(R"(
        CREATE INDEX `accounts_sort_index` ON `accounts`(`group_id`, `first_name`, `last_name`)
    )");
}

void Database::createPreparedStatements()
{
    _insertGroupQuery = new QSqlQuery(_db);
    _insertGroupQuery->prepare(R"(
        INSERT OR IGNORE INTO `groups`(`group_name`, `group_order`)
        VALUES (?, ?)
    )");

    _insertAccountQuery = new QSqlQuery(_db);
    _insertAccountQuery->prepare(R"(
        INSERT OR IGNORE INTO `accounts`
        (
            `id`, `username`, `first_name`, `last_name`, `sex`, `country`, `language`, `birthday`, `group_id`, `roaster_id`
        )
        VALUES
        (
            :id, :username, :first_name, :last_name, :sex, :country, :language, :birthday, :group_id, :roaster_id
        )
    )");
}
