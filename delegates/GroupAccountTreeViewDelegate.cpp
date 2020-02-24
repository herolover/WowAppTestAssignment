#include "GroupAccountTreeViewDelegate.h"

#include <QApplication>
#include <QPainter>
#include <QRect>
#include <QVariant>

GroupAccountTreeViewDelegate::GroupAccountTreeViewDelegate(AvatarManager &avatarManager, QObject *parent)
    : QStyledItemDelegate(parent)
    , _avatarManager(avatarManager)
{
}

void GroupAccountTreeViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.parent().isValid()) {
        auto groupName = index.siblingAtColumn(1).data().toString();
        auto groupSize = index.model()->rowCount(index);
        painter->drawText(option.rect, Qt::AlignVCenter, QString("%1 (%2)").arg(groupName).arg(groupSize));
    } else {
        auto firstName = index.siblingAtColumn(1).data().toString();
        auto lastName = index.siblingAtColumn(2).data().toString();
        auto sex = static_cast<Sex>(index.siblingAtColumn(3).data().value<int>());
        auto avatar = _avatarManager.getAccountAvatar(firstName, lastName, sex);
        painter->drawImage(QRect(option.rect.x() - 34, option.rect.y(), 32, 32), avatar);

        QStyleOptionViewItem customOption = option;
        customOption.text = QString("%1 %2").arg(firstName, lastName);
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &customOption, painter);
    }
}

QSize GroupAccountTreeViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    return QSize(100, 36);
}

bool GroupAccountTreeViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (index.parent().isValid()) {
        emit detailInfoRequested(index);
    }

    return false;
}
