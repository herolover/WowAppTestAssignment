#include "FileImporter.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QtConcurrent>

namespace
{
    QByteArray readWholeFile(const QString &filename)
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        return file.readAll();
    }

    Sex convertSex(const QString &stringSex)
    {
        if (stringSex == "MALE") {
            return Sex::MALE;
        } else if (stringSex == "FEMALE") {
            return Sex::FEMALE;
        } else {
            return Sex::UNKNOWN;
        }
    }

    Group convertGroup(const QJsonObject &rosterObject)
    {
        Group group;
        group.groupName = rosterObject["group"].toString();
        group.groupOrder = rosterObject["groupOrder"].toInt();

        return group;
    }

    Account convertAccount(const QJsonObject &rosterObject, const QJsonObject &accountObject)
    {
        Account account;
        account.id = accountObject["id"].toString();
        account.username = accountObject["username"].toString();
        account.firstName = accountObject["firstName"].toString();
        account.lastName = accountObject["lastName"].toString();
        account.sex = convertSex(accountObject["sex"].toString());
        account.country = accountObject["country"].toString();
        account.language = accountObject["language"].toString();
        account.birthday = static_cast<qint64>(accountObject["birthday"].toDouble()) * 1000; // convert to ms
        account.rosterId = rosterObject["id"].toString();

        return account;
    }
}

FileImporter::FileImporter(Database &db, QObject *parent)
    : QObject(parent)
    , _db(db)
{
}

FileImporter::~FileImporter()
{
    abortImporting();
    _threadFuture.waitForFinished();
}

FileImporter::ImportStatus FileImporter::importStatus() const
{
    return _importStatus;
}

void FileImporter::importFile(const QString &filename)
{
    auto status = importStatus();
    if (status == READY || status == ABORTED || status == INVALID_FILE || status == FINISHED) {
        setImportStatus(LOADING);
        _isAborted = false;

        _threadFuture = QtConcurrent::run([this, filename]() {
            if (_isAborted) {
                return setImportStatus(ABORTED);
            }

            auto fileData = readWholeFile(filename);
            qDebug() << fileData.size();
            if (_isAborted) {
                return setImportStatus(ABORTED);
            }

            setImportStatus(PARSING);
            // certanly we should use a stream json parser like rapidjson library, but Qt doesn't provide such functionality
            QJsonDocument json = QJsonDocument::fromJson(fileData);
            if (json.isNull()) {
                return setImportStatus(INVALID_FILE);
            }
            if (_isAborted) {
                return setImportStatus(ABORTED);
            }

            setImportStatus(IMPORTING);
            QJsonArray roster = json.object()["roster"].toArray();
            qDebug() << roster.size();
            qint64 accountsProcessed = 0;
            qint64 accountsTotal = roster.size();
            qint64 accountsNew = 0;
            for (auto &&item : roster) {
                auto rosterObject = item.toObject();
                auto group = convertGroup(rosterObject);
                auto groupId = _db.addGroup(group);

                auto accountObject = rosterObject["account"].toObject();
                auto account = convertAccount(rosterObject, accountObject);
                account.groupId = groupId;
                if (_db.addAccount(account)) {
                    ++accountsNew;
                }
                ++accountsProcessed;

                emit importProgress(accountsProcessed, accountsTotal, accountsNew);

                QThread::usleep(1000);

                if (_isAborted) {
                    return setImportStatus(ABORTED);
                }
            }

            setImportStatus(FINISHED);
        });
    }
}

void FileImporter::abortImporting()
{
    _isAborted = true;
}

void FileImporter::setImportStatus(ImportStatus importStatus)
{
    if (_importStatus == importStatus) {
        return;
    }

    qDebug() << importStatus;

    _importStatus = importStatus;
    emit importStatusChanged(importStatus);
}
