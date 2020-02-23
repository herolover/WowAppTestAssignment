#pragma once

#include "../FileImporter.h"

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>

class FileImporterWidget: public QWidget
{
    Q_OBJECT

public:
    FileImporterWidget(Database &db, QWidget *parent = nullptr);

    void importFromFile(const QString &filename);

private:
    void createWidgets();
    void createLayout();

    FileImporter *_fileImporter;

    QProgressBar *_progressBar;
    QLabel *_importStatusLabel;
    QLabel *_newAccountsLabel;
    QPushButton *_abortButton;
};
