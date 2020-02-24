#pragma once

#include <QSqlQueryModel>
#include <QSqlQuery>

class AccountListModel: public QSqlQueryModel
{
    Q_OBJECT

    Q_PROPERTY(QString likeFilter READ likeFilter WRITE setLikeFilter NOTIFY likeFilterChanged)

public:
    AccountListModel(QObject *parent = nullptr);

    qint64 totalSize() const;
    QSqlQuery sizeQuery() const;
    void setSizeQuery(QSqlQuery sizeQuery);

    QString likeFilter() const;
    void setLikeFilter(const QString &likeFilter);

signals:
    void likeFilterChanged(QString likeFilter);

private:
    QSqlQuery _sizeQuery;
    qint64 _totalSize;
    QString _likeFilter;
};
