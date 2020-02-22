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

bool ImportFromURLDialog::isDownloading() const
{
    return _isDownloading;
}

void ImportFromURLDialog::setIsDownloading(bool isDownloading)
{
    if (_isDownloading == isDownloading)
        return;

    _isDownloading = isDownloading;
    emit isDownloadingChanged(_isDownloading);
}

void ImportFromURLDialog::createWidgets()
{
    _urlEdit = new QLineEdit(this);
    _urlEdit->setPlaceholderText(tr("https://"));
    _urlEdit->setText("https://file.wowapp.me/owncloud/index.php/s/sGOXibS0ZSspQE8/download");

    _importButton = new QPushButton(tr("Import"), this);
    connect(_importButton, &QPushButton::clicked, this, &ImportFromURLDialog::onImport);

    _progressBar = new QProgressBar(this);

    connect(this, &ImportFromURLDialog::isDownloadingChanged, [this](bool isDownloading) {
        if (isDownloading) {
            _importButton->setText(tr("Cancel"));
            _progressBar->reset();
        } else {
            _importButton->setText(tr("Import"));
        }
        _urlEdit->setDisabled(isDownloading);
        _progressBar->setVisible(isDownloading);
    });
    setIsDownloading(false);

    _networkManager = new QNetworkAccessManager(this);
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

void ImportFromURLDialog::onImport()
{
    if (!isDownloading()) {
        setIsDownloading(true);
        _currentReply = _networkManager->get(QNetworkRequest(_urlEdit->text()));

        connect(_currentReply, &QNetworkReply::downloadProgress, [this](qint64 bytesReceived, qint64 totalBytes) {
            _progressBar->setMaximum(100);
            _progressBar->setValue(static_cast<double>(bytesReceived) / totalBytes * 100);
        });
        connect(_networkManager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply) {
            setIsDownloading(false);
            reply->deleteLater();
        });
    } else {
        _currentReply->abort();
    }
}
