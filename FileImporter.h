#pragma once

#include "Database.h"

#include <QFuture>

#include <atomic>

class FileImporter: public QObject
{
    Q_OBJECT

    Q_PROPERTY(ImportStatus importStatus READ importStatus NOTIFY importStatusChanged)

public:
    enum ImportStatus
    {
        READY,
        LOADING,
        PARSING,
        IMPORTING,
        INVALID_FILE,
        ABORTED,
        FINISHED
    };
    Q_ENUM(ImportStatus)

    FileImporter(Database &db, QObject *parent = nullptr);
    ~FileImporter();

    ImportStatus importStatus() const;

    void importFile(const QString &filename);
    void abortImporting();

signals:
    void importStatusChanged(ImportStatus importStatus);
    void importProgress(qint64 accountsProcessed, quint32 accountsTotal, qint64 accountsNew);

private:
    void setImportStatus(ImportStatus importStatus);

    Database &_db;
    ImportStatus _importStatus = READY;
    std::atomic<bool> _isAborted;
    QFuture<void> _threadFuture;
};
