#include "GroupAccountTreeModel.h"

#include <QDebug>

GroupAccountTreeModel::GroupAccountTreeModel(Database &db, QObject *parent)
    : QAbstractItemModel(parent)
    , _db(db)
{
    resetModel();

    connect(&_db, &Database::groupAccountUpdated, [this]() {
        resetModel();
    });
}

void GroupAccountTreeModel::setLikeFilter(const QString &likeFilter)
{
    _likeFilter = likeFilter;
    beginResetModel();
    for (auto &accountListModel : _accountModel) {
        accountListModel->setLikeFilter(likeFilter);
    }
    endResetModel();
}

void GroupAccountTreeModel::resetModel()
{
    beginResetModel();
    delete _groupModel;
    for (auto &accountModel : _accountModel) {
        delete accountModel;
    }
    _groupModel = _db.createGroupListModel();
    _accountModel = _db.createAccountListModelList(_groupModel);
    for (auto &accountListModel : _accountModel) {
        accountListModel->setLikeFilter(_likeFilter);
    }
    endResetModel();
}

QModelIndex GroupAccountTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return createIndex(row, column, -1);
    } else if (!parent.parent().isValid()) {
        return createIndex(row, column, quintptr(parent.row()));
    } else {
        return QModelIndex();
    }
}

QModelIndex GroupAccountTreeModel::parent(const QModelIndex &child) const
{
    if (child.internalId() != static_cast<quintptr>(-1)) {
        int parentRow = child.internalId();
        return createIndex(parentRow, 0, -1);
    } else {
        return QModelIndex();
    }
}

int GroupAccountTreeModel::rowCount(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return _groupModel->rowCount();
    } else if (!index.parent().isValid()) {
        return _accountModel[index.row()]->rowCount();
    } else {
        return 0;
    }
}

int GroupAccountTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant GroupAccountTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.parent().isValid()) {
        return _groupModel->data(index, role);
    } else if (!index.parent().parent().isValid()) {
        auto model = _accountModel[index.parent().row()];
        auto lastRowCount = model->rowCount();
        auto k = index.row() / static_cast<double>(lastRowCount);
        if (k > 0.9 && model->canFetchMore()) {
            model->fetchMore();

            // sorry for this hack
            auto nonConstThis = const_cast<GroupAccountTreeModel *>(this);
            nonConstThis->beginInsertRows(index.parent(), lastRowCount, model->rowCount());
            nonConstThis->endInsertRows();
        }

        return _accountModel[index.parent().row()]->data(index, role);
    } else {
        return "";
    }
}
