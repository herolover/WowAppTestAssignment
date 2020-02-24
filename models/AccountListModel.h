#pragma once

#include <QSqlQueryModel>

class AccountListModel: public QSqlQueryModel
{
    Q_OBJECT

    Q_PROPERTY(QString likeFilter READ likeFilter WRITE setLikeFilter NOTIFY likeFilterChanged)

public:
    AccountListModel(QObject *parent = nullptr);

    QString likeFilter() const;
    void setLikeFilter(const QString &likeFilter);

signals:
    void likeFilterChanged(QString likeFilter);

private:
    QString _likeFilter;
};
