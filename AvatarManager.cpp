#include "AvatarManager.h"

#include <QApplication>
#include <QBrush>
#include <QDir>
#include <QPainter>
#include <QPen>
#include <QRect>

namespace
{
    QColor sexColor(Sex sex)
    {
        switch (sex) {
        case Sex::MALE:
            return QColor::fromRgb(0xB5E6FF);
        case Sex::FEMALE:
            return QColor::fromRgb(0xFCD0FC);
        case Sex::UNKNOWN:
            return QColor::fromRgb(0xE1E8ED);
        }
    }
}

AvatarManager::AvatarManager(const QString &avatarDirectory, qint32 avatarSize)
    : _avatarSize(avatarSize)
{
    QDir dir(QCoreApplication::applicationDirPath());
    if (!dir.exists(avatarDirectory)) {
        dir.mkpath(avatarDirectory);
    }
    _avatarDirectory = dir.absoluteFilePath(avatarDirectory);
}

QImage AvatarManager::getAccountAvatar(const QString &accountId, const QString &firstName, const QString &lastName, Sex sex)
{
    QDir dir(_avatarDirectory);
    auto filename = makeFilename(accountId);
    if (dir.exists(filename)) {
        return QImage(dir.absoluteFilePath(filename));
    } else {
        auto image = createAvatar(firstName, lastName, sex);
        image.save(dir.absoluteFilePath(filename));
        return image;
    }
}

QString AvatarManager::makeFilename(const QString &accountId)
{
    return accountId + ".png";
}

QImage AvatarManager::createAvatar(const QString &firstName, const QString &lastName, Sex sex)
{
    QImage avatar(_avatarSize, _avatarSize, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&avatar);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect rect(0, 0, _avatarSize, _avatarSize);

    painter.setPen(QPen(Qt::PenStyle::NoPen));
    painter.setBrush(QBrush(sexColor(sex)));
    painter.drawEllipse(rect);

    QFont font("Tahoma");
    font.setPixelSize(_avatarSize * 0.6);
    font.setCapitalization(QFont::AllUppercase);
    painter.setFont(font);
    painter.setPen(QPen(QColorConstants::DarkGray));
    painter.drawText(rect, Qt::AlignCenter,
                     QString("%1%2").arg(firstName.size() > 0 ? firstName.front() : ' ',
                                         lastName.size() > 0 ? lastName.front() : ' '));

    return avatar;
}
