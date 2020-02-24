#include "AccountListModel.h"

AccountListModel::AccountListModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

QString AccountListModel::likeFilter() const
{
    return _likeFilter;
}

void AccountListModel::setLikeFilter(const QString &likeFilter)
{
    if (_likeFilter == likeFilter) {
        return;
    }

    _likeFilter = likeFilter;
    emit likeFilterChanged(likeFilter);
}
