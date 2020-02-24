#include "AccountListModel.h"

AccountListModel::AccountListModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

qint64 AccountListModel::totalSize() const
{
    return _totalSize;
}

QSqlQuery AccountListModel::sizeQuery() const
{
    return _sizeQuery;
}

void AccountListModel::setSizeQuery(QSqlQuery sizeQuery)
{
    _sizeQuery = sizeQuery;
    _sizeQuery.exec();
    _sizeQuery.first();
    _totalSize = _sizeQuery.value(0).value<qint64>();
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
