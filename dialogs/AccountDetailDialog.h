#pragma once

#include "../AvatarManager.h"

#include <QDialog>
#include <QLabel>
#include <QModelIndex>

class AccountDetailDialog: public QDialog
{
public:
    AccountDetailDialog(QModelIndex index, AvatarManager &avatarManager, QWidget *parent = nullptr);

private:
    void createWidgets(QModelIndex index);

    AvatarManager &_avatarManager;
};
