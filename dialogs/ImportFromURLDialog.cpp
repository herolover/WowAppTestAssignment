#include "ImportFromURLDialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

ImportFromURLDialog::ImportFromURLDialog(QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Import From URL"));
    setAttribute(Qt::WA_DeleteOnClose);

    createWidgets();
    createLayout();
}

void ImportFromURLDialog::createWidgets()
{
    _urlEdit = new QLineEdit(this);
    _urlEdit->setPlaceholderText(tr("https://"));
    _urlEdit->setText("https://file.wowapp.me/owncloud/index.php/s/sGOXibS0ZSspQE8/download");

    _importButton = new QPushButton(tr("Import"), this);
    connect(_importButton, &QPushButton::clicked, this, &ImportFromURLDialog::onImport);

    _progressBar = new QProgressBar(this);

    _fileDownloader = new FileDownloader("./temp_downloads", this);
    connect(_fileDownloader, &FileDownloader::isDownloadingChanged, this, &ImportFromURLDialog::onIsDownloadingChanged);
    connect(_fileDownloader, &FileDownloader::downloadProgress, this, &ImportFromURLDialog::onDownloadProgress);
    connect(_fileDownloader, &FileDownloader::downloadFinished, this, &ImportFromURLDialog::onDownloadFinished);

    onIsDownloadingChanged(false);
}

void ImportFromURLDialog::createLayout()
{
    auto *urlLayout = new QHBoxLayout();
    urlLayout->addWidget(_urlEdit);
    urlLayout->addWidget(_importButton);

    auto *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(urlLayout);
    mainLayout->addWidget(_progressBar);

    setLayout(mainLayout);
    setMinimumWidth(500);
}

void ImportFromURLDialog::onIsDownloadingChanged(bool isDownloading)
{
    if (isDownloading) {
        _importButton->setText(tr("Cancel"));
        _progressBar->reset();
    } else {
        _importButton->setText(tr("Import"));
    }
    _urlEdit->setDisabled(isDownloading);
    _progressBar->setVisible(isDownloading);
}

void ImportFromURLDialog::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    _progressBar->setMaximum(100);
    _progressBar->setValue(static_cast<double>(bytesReceived) / bytesTotal * 100);
}

void ImportFromURLDialog::onDownloadFinished(const QString &filepath, bool isAborted)
{
    if (!isAborted) {
        qDebug() << filepath << isAborted;
    }
}

void ImportFromURLDialog::onImport()
{
    if (!_fileDownloader->isDownloading()) {
        _fileDownloader->downloadFile(_urlEdit->text());
    } else {
        _fileDownloader->abortDownloading();
    }
}
