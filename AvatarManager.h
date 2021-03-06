#pragma once

#include "models/Sex.h"

#include <QImage>

class AvatarManager
{
public:
    AvatarManager(const QString &avatarDirectory, qint32 avatarSize);

    QImage getAccountAvatar(const QString &firstName, const QString &lastName, Sex sex);

private:
    QString makeFilename(const QString &firstName, const QString &lastName, Sex sex);
    QImage createAvatar(const QString &firstName, const QString &lastName, Sex sex);

    QString _avatarDirectory;
    qint32 _avatarSize;
};
