#pragma once

#include "GroupId.h"
#include "Sex.h"

#include <QString>

struct Account
{
    QString id;
    QString username;
    QString firstName;
    QString lastName;
    Sex sex;
    QString country;
    QString language;
    qint64 birthday;
    GroupId groupId;
    QString roasterId;
};
