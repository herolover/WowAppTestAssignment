#pragma once

#include "GroupId.h"

#include <QString>

struct Group
{
    GroupId groupId;
    QString groupName;
    qint8 groupOrder;
};
