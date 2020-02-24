#pragma once

#include "GroupId.h"
#include "../Database.h"

#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QSqlQueryModel>
#include <QVector>

class GroupAccountTreeModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    GroupAccountTreeModel(Database &db, QObject *parent = nullptr);

    void setLikeFilter(const QString &likeFilter);

    void resetModel();

private:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &index = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Database &_db;
    QString _likeFilter;
    QSqlQueryModel *_groupModel = nullptr;
    QVector<AccountListModel *> _accountModel;
};
