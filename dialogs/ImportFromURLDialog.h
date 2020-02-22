#pragma once

#include "../FileDownloader.h"

#include <QDialog>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>

class ImportFromURLDialog: public QDialog
{
    Q_OBJECT

public:
    ImportFromURLDialog(QWidget *parent = nullptr);

private:
    void createWidgets();
    void createLayout();

    void onIsDownloadingChanged(bool isDownloading);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished(const QString &filename, bool isAborted);
    void onImport();

    QLineEdit *_urlEdit;
    QPushButton *_importButton;
    QProgressBar *_progressBar;
    FileDownloader *_fileDownloader;
};


