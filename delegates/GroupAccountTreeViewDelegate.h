#pragma once

#include "../AvatarManager.h"

#include <QStyledItemDelegate>

class GroupAccountTreeViewDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    GroupAccountTreeViewDelegate(AvatarManager &avatarManager, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void detailInfoRequested(QModelIndex index);

private:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    AvatarManager &_avatarManager;
};
