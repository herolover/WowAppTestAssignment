#include "FileImporterWidget.h"

#include <QGridLayout>
#include <QDebug>

FileImporterWidget::FileImporterWidget(Database &db, QWidget *parent)
    : QWidget(parent)
{
    _fileImporter = new FileImporter(db, this);

    createWidgets();
    createLayout();
}

void FileImporterWidget::importFromFile(const QString &filename)
{
    _progressBar->reset();
    _newAccountsLabel->setText("");
    _fileImporter->importFile(filename);
}

void FileImporterWidget::createWidgets()
{
    _progressBar = new QProgressBar(this);
    _progressBar->setFormat("%v/%m");

    _importStatusLabel = new QLabel(this);
    _newAccountsLabel = new QLabel(this);

    QObject::connect(_fileImporter, &FileImporter::importProgress, this, [this](qint64 accountsProcessed, quint32 accountsTotal, qint64 accountsNew) {
        _progressBar->setMaximum(accountsTotal);
        _progressBar->setValue(accountsProcessed);

        _newAccountsLabel->setText(tr("New accounts: ") + QString::number(accountsNew));
    }, Qt::QueuedConnection);
    QObject::connect(_fileImporter, &FileImporter::importStatusChanged, this, [this](FileImporter::ImportStatus importStatus) {
        switch (importStatus) {
        case FileImporter::READY:
            _importStatusLabel->setText(tr("Ready"));
            break;
        case FileImporter::LOADING:
            _importStatusLabel->setText(tr("Loading"));
            break;
        case FileImporter::PARSING:
            _importStatusLabel->setText(tr("Parsing"));
            break;
        case FileImporter::IMPORTING:
            _importStatusLabel->setText(tr("Importing"));
            break;
        case FileImporter::INVALID_FILE:
            _importStatusLabel->setText(tr("Invalid File"));
            break;
        case FileImporter::ABORTED:
            _importStatusLabel->setText(tr("Aborted"));
            break;
        case FileImporter::FINISHED:
            _importStatusLabel->setText(tr("Finished"));
            break;
        }

        _abortButton->setVisible(importStatus == FileImporter::LOADING || importStatus == FileImporter::PARSING || importStatus == FileImporter::IMPORTING);
        _newAccountsLabel->setVisible(importStatus == FileImporter::IMPORTING || importStatus == FileImporter::ABORTED || importStatus == FileImporter::FINISHED);
        _progressBar->setVisible(importStatus != FileImporter::READY && importStatus != FileImporter::INVALID_FILE);
    }, Qt::QueuedConnection);

    _abortButton = new QPushButton(tr("Abort"), this);
    connect(_abortButton, &QPushButton::clicked, [this]() {
        _fileImporter->abortImporting();
    });
}

void FileImporterWidget::createLayout()
{
    auto *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(_importStatusLabel, 0, 0, 1, 2, Qt::AlignHCenter);
    mainLayout->addWidget(_newAccountsLabel, 1, 0, 1, 2, Qt::AlignHCenter);
    mainLayout->addWidget(_progressBar, 2, 0);
    mainLayout->addWidget(_abortButton, 2, 1, Qt::AlignRight);

    setLayout(mainLayout);
}
