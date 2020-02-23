#pragma once

#include "Database.h"
#include "dialogs/ImportFromURLDialog.h"
#include "dialogs/ImportFromFileDialog.h"

#include <QMainWindow>
#include <QTreeView>

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void createWidgets();
    void createLayout();

    Database _db;

    QTreeView *_accountsListView;

    ImportFromURLDialog *_importFromURLDialog;
    ImportFromFileDialog *_importFromFileDialog;
};
