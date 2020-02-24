#include "MainWindow.h"

#include "delegates/GroupAccountTreeViewDelegate.h"
#include "dialogs/ImportFromURLDialog.h"
#include "models/GroupAccountTreeModel.h"

#include <QMenuBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _db("WowAppTestAssignment.db")
    , _avatarManager32("avatars/32", 32)
    , _avatarManager128("avatars/128", 128)
{
    auto *importMenu = menuBar()->addMenu(tr("Import"));
    auto *fromUrl = importMenu->addAction(tr("From URL"));
    auto *fromFile = importMenu->addAction(tr("From File"));

    connect(fromUrl, &QAction::triggered, [this]() {
        _importFromURLDialog->show();
    });
    connect(fromFile, &QAction::triggered, [this]() {
        _importFromFileDialog->show();
    });

    createWidgets();
}

void MainWindow::createWidgets()
{
    _groupAccountTreeViewWidget = new GroupAccountTreeViewWidget(new GroupAccountTreeViewDelegate(_avatarManager32, this),
                                                                 new GroupAccountTreeModel(_db, this),
                                                                 this);
    setCentralWidget(_groupAccountTreeViewWidget);

    _importFromURLDialog = new ImportFromURLDialog(_db, this);
    _importFromFileDialog = new ImportFromFileDialog(_db, this);

    resize(450, 600);
}
