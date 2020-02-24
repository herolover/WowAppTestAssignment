#include "GroupAccountTreeViewWidget.h"

#include <QVBoxLayout>

GroupAccountTreeViewWidget::GroupAccountTreeViewWidget(GroupAccountTreeViewDelegate *delegate, GroupAccountTreeModel *model, QWidget *parent)
    : QWidget(parent)
{
    createWidgets(delegate, model);
    createLayout();
}

void GroupAccountTreeViewWidget::createWidgets(GroupAccountTreeViewDelegate *delegate, GroupAccountTreeModel *model)
{
    _filterEdit = new QLineEdit(this);
    _filterEdit->setPlaceholderText(tr("Filter"));

    _groupAccountTreeView = new QTreeView(this);
    _groupAccountTreeView->setHeaderHidden(true);
    _groupAccountTreeView->setModel(model);
    _groupAccountTreeView->setItemDelegate(delegate);

    connect(_filterEdit, &QLineEdit::textChanged, [this, model](const QString &text) {
        model->setLikeFilter(text);
        _groupAccountTreeView->expandAll();
    });
    connect(_filterEdit, &QLineEdit::returnPressed, [this]() {
        _filterEdit->selectAll();
    });
    connect(_groupAccountTreeView, &QTreeView::doubleClicked, this, &GroupAccountTreeViewWidget::treeItemDoubleClicked);
}

void GroupAccountTreeViewWidget::createLayout()
{
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(_filterEdit);
    layout->addWidget(_groupAccountTreeView);

    setLayout(layout);
}
