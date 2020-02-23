#include "ImportFromFileDialog.h"

#include <QFileDialog>
#include <QGridLayout>

ImportFromFileDialog::ImportFromFileDialog(Database &db, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Import From File"));

    _fileImporterWidget = new FileImporterWidget(db, this);
    _fileImporterWidget->hide();

    createWidgets();
    createLayout();
}

void ImportFromFileDialog::createWidgets()
{
    _fileEdit = new QLineEdit(this);
    _fileEdit->setReadOnly(true);

    _openImportButton = new QPushButton(tr("Select File"), this);
    connect(_openImportButton, &QPushButton::clicked, [this]() {
        QFileDialog fileDialog(this, tr("Open roster json"), ".", "JSON File (*.json)");
        fileDialog.setFileMode(QFileDialog::ExistingFile);
        if (fileDialog.exec() == QDialog::Accepted) {
            _fileEdit->setText(fileDialog.selectedFiles()[0]);

            _fileImporterWidget->show();
            _fileImporterWidget->importFromFile(_fileEdit->text());
        }
    });
}

void ImportFromFileDialog::createLayout()
{
    auto *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(_fileEdit, 0, 0);
    mainLayout->addWidget(_openImportButton, 0, 1);
    mainLayout->addWidget(_fileImporterWidget, 1, 0, 1, 2);

    setLayout(mainLayout);
    setMinimumWidth(500);
}
