#pragma once

#include "../delegates/GroupAccountTreeViewDelegate.h"
#include "../models/GroupAccountTreeModel.h"

#include <QLineEdit>
#include <QTreeView>
#include <QWidget>

class GroupAccountTreeViewWidget: public QWidget
{
    Q_OBJECT
public:
    GroupAccountTreeViewWidget(GroupAccountTreeViewDelegate *delegate, GroupAccountTreeModel *model, QWidget *parent = nullptr);

signals:

private:
    void createWidgets(GroupAccountTreeViewDelegate *delegate, GroupAccountTreeModel *model);
    void createLayout();

    QLineEdit *_filterEdit;
    QTreeView *_groupAccountTreeView;
};
