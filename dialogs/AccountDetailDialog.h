#pragma once

#include "../AvatarManager.h"

#include <QDialog>
#include <QLabel>
#include <QModelIndex>

class AccountDetailDialog: public QDialog
{
public:
    AccountDetailDialog(const QModelIndex &index, AvatarManager &avatarManager, QWidget *parent = nullptr);

private:
    void createWidgets(const QModelIndex &index);

    AvatarManager &_avatarManager;
};
