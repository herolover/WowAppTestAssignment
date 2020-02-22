#include "MainWindow.h"

#include "dialogs/ImportFromURLDialog.h"

#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *importMenu = menuBar()->addMenu(tr("Import"));
    auto *fromUrl = importMenu->addAction(tr("From URL"));
    auto *fromFile = importMenu->addAction(tr("From File"));

    connect(fromUrl, &QAction::triggered, this, &MainWindow::importFromUrl);
    connect(fromFile, &QAction::triggered, this, &MainWindow::importFromFile);
}

MainWindow::~MainWindow()
{
}

void MainWindow::importFromUrl()
{
    auto *importFromURLDialog = new ImportFromURLDialog(this);
    importFromURLDialog->show();
}

void MainWindow::importFromFile()
{

}

