#include "MainWindow.h"

#include "dialogs/ImportFromURLDialog.h"
#include "models/GroupTreeModel.h"

#include <QMenuBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _db("WowAppTestAssignment.db")
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
    createLayout();
}

void MainWindow::createWidgets()
{
    auto *model = new GroupTreeModel();
    model->_groupModel = _db._groupTable;
    model->_accountModel = _db.createAccountModelList();

    _accountsListView = new QTreeView(this);
    _accountsListView->setModel(model);

    setCentralWidget(_accountsListView);

    _importFromURLDialog = new ImportFromURLDialog(_db, this);
    _importFromFileDialog = new ImportFromFileDialog(_db, this);

//    _a = new QTableView(this);
//    _a->setModel(_db._accountsModel);
}

void MainWindow::createLayout()
{
//    auto *mainLayout = new QVBoxLayout(this);
//    mainLayout->addWidget(_accountsListView);
//    mainLayout->addWidget(_a);

//    setLayout(mainLayout);
}
