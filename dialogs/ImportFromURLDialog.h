#pragma once

#include "../FileDownloader.h"
#include "../widgets/FileImporterWidget.h"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>

class ImportFromURLDialog: public QDialog
{
    Q_OBJECT

public:
    ImportFromURLDialog(Database &db, QWidget *parent = nullptr);

private:
    void createWidgets();
    void createLayout();

    void onIsDownloadingChanged(bool isDownloading);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished(const QString &filename, bool isAborted);

    QLineEdit *_urlEdit;
    QPushButton *_importButton;
    QLabel *_downloadingLabel;
    QProgressBar *_progressBar;
    FileDownloader *_fileDownloader;

    FileImporterWidget *_fileImporterWidget;
};


