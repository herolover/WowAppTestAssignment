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

private:
    AvatarManager &_avatarManager;
};
