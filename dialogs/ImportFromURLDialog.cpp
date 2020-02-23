#include "ImportFromURLDialog.h"

#include <QGridLayout>
#include <QDebug>

ImportFromURLDialog::ImportFromURLDialog(Database &db, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Import From URL"));

    _fileImporterWidget = new FileImporterWidget(db, this);
    _fileImporterWidget->hide();

    createWidgets();
    createLayout();
}

void ImportFromURLDialog::createWidgets()
{
    _urlEdit = new QLineEdit(this);
    _urlEdit->setPlaceholderText(tr("https://"));
    _urlEdit->setText("https://file.wowapp.me/owncloud/index.php/s/sGOXibS0ZSspQE8/download");

    _importButton = new QPushButton(tr("Import"), this);
    connect(_importButton, &QPushButton::clicked, [this]() {
        if (!_fileDownloader->isDownloading()) {
            _fileDownloader->downloadFile(_urlEdit->text());
        } else {
            _fileDownloader->abortDownloading();
        }
    });

    _downloadingLabel = new QLabel(tr("Downloading"), this);
    _progressBar = new QProgressBar(this);

    _fileDownloader = new FileDownloader("./temp_downloads", this);
    connect(_fileDownloader, &FileDownloader::isDownloadingChanged, this, &ImportFromURLDialog::onIsDownloadingChanged);
    connect(_fileDownloader, &FileDownloader::downloadProgress, this, &ImportFromURLDialog::onDownloadProgress);
    connect(_fileDownloader, &FileDownloader::downloadFinished, this, &ImportFromURLDialog::onDownloadFinished);

    onIsDownloadingChanged(false);
}

void ImportFromURLDialog::createLayout()
{
    auto *mainLayout = new QGridLayout();
    mainLayout->addWidget(_urlEdit, 0, 0);
    mainLayout->addWidget(_importButton, 0, 1);
    mainLayout->addWidget(_downloadingLabel, 1, 0, 1, 2, Qt::AlignHCenter);
    mainLayout->addWidget(_progressBar, 2, 0, 1, 2);
    mainLayout->addWidget(_fileImporterWidget, 3, 0, 1, 2);

    setLayout(mainLayout);
    setMinimumWidth(500);
}

void ImportFromURLDialog::onIsDownloadingChanged(bool isDownloading)
{
    if (isDownloading) {
        _importButton->setText(tr("Cancel"));
        _progressBar->reset();
        _fileImporterWidget->hide();
    } else {
        _importButton->setText(tr("Import"));
    }
    _urlEdit->setDisabled(isDownloading);
    _downloadingLabel->setVisible(isDownloading);
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

        _fileImporterWidget->show();
        _fileImporterWidget->importFromFile(filepath);
    }
}
