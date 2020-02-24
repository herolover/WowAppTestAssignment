#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

#include <memory>

class FileDownloader: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isDownloading READ isDownloading NOTIFY isDownloadingChanged)
public:
    FileDownloader(const QString &downloadDirectory = "temp_downloads", QObject *parent = nullptr);
    ~FileDownloader();

    bool isDownloading() const;

    void downloadFile(const QString &url);
    void abortDownloading();

signals:
    void isDownloadingChanged(bool isDownloading);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(const QString &filepath, bool isAborted);

private:
    void checkDownloadDirectory(const QString &downloadDirectory);
    void setIsDownloading(bool isDownloading);

    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    QNetworkAccessManager *_networkManager;
    QNetworkReply *_currentReply = nullptr;

    QString _downloadDirectory;
    std::unique_ptr<QFile> _file;
    bool _isDownloading = false;
    bool _isAborted;
};
