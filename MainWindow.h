#pragma once

#include "AvatarManager.h"
#include "Database.h"
#include "dialogs/ImportFromFileDialog.h"
#include "dialogs/ImportFromURLDialog.h"
#include "widgets/GroupAccountTreeViewWidget.h"

#include <QMainWindow>

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void createWidgets();

    Database _db;

    AvatarManager _avatarManager32;
    AvatarManager _avatarManager128;

    GroupAccountTreeViewWidget *_groupAccountTreeViewWidget;

    ImportFromURLDialog *_importFromURLDialog;
    ImportFromFileDialog *_importFromFileDialog;
};
