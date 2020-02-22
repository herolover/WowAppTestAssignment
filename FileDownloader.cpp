#include "FileDownloader.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

namespace
{
    QString getFilename(QNetworkReply *reply)
    {
        if (reply) {
            auto contentDisposition = reply->header(QNetworkRequest::ContentDispositionHeader).value<QString>();
            if (!contentDisposition.isEmpty()) {
                QRegularExpression re("filename=\"(.*)\"");
                QRegularExpressionMatch match = re.match(contentDisposition);
                if (match.hasMatch()) {
                    return match.captured(1);
                }
            } else {
                return reply->url().fileName();
            }
        }

        return "unknown";
    }

    std::unique_ptr<QFile> openFileInDirectory(const QString &directory, const QString &filename)
    {
        QDir dir(directory);
        auto file = std::make_unique<QFile>(dir.absoluteFilePath(filename));
        file->open(QIODevice::WriteOnly | QIODevice::Truncate);

        return file;
    }
}

FileDownloader::FileDownloader(const QString &downloadDirectory, QObject *parent)
    : QObject(parent)
{
    checkDownloadDirectory(downloadDirectory);

    _networkManager = new QNetworkAccessManager(this);
}

bool FileDownloader::isDownloading() const
{
    return _isDownloading;
}

void FileDownloader::setIsDownloading(bool isDownloading)
{
    if (_isDownloading == isDownloading) {
        return;
    }

    _isDownloading = isDownloading;
    emit isDownloadingChanged(_isDownloading);
}

void FileDownloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (!_file) {
        _file = openFileInDirectory(_downloadDirectory, getFilename(_currentReply));
    }

    auto data = _currentReply->readAll();
    _file->write(data);

    emit downloadProgress(bytesReceived, bytesTotal);
}

void FileDownloader::downloadFile(const QString &url)
{
    if (!isDownloading()) {
        setIsDownloading(true);
        _isAborted = false;

        _currentReply = _networkManager->get(QNetworkRequest(url));

        connect(_currentReply, &QNetworkReply::downloadProgress, this, &FileDownloader::onDownloadProgress);
        connect(_currentReply, &QNetworkReply::finished, [this]() {
            setIsDownloading(false);
            _currentReply->deleteLater();
            _currentReply = nullptr;

            QFileInfo fileInfo(*_file);
            emit downloadFinished(fileInfo.absoluteFilePath(), _isAborted);

            _file.reset();
        });
    }
}

void FileDownloader::abortDownloading()
{
    if (_currentReply) {
        _currentReply->abort();
        _isAborted = true;
    }
}

void FileDownloader::checkDownloadDirectory(const QString &downloadDirectory)
{
    QDir dir(QCoreApplication::applicationDirPath());
    if (!dir.exists(downloadDirectory)) {
        dir.mkpath(downloadDirectory);
    }
    _downloadDirectory = dir.absoluteFilePath(downloadDirectory);
}
