#pragma once

#include "../widgets/FileImporterWidget.h"

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class ImportFromFileDialog: public QDialog
{
    Q_OBJECT

public:
    ImportFromFileDialog(Database &db, QWidget *parent = nullptr);

private:
    void createWidgets();
    void createLayout();

    FileImporterWidget *_fileImporterWidget;

    QLineEdit *_fileEdit;
    QPushButton *_openImportButton;
};
