#include "AccountDetailDialog.h"

#include <QDateTime>
#include <QGridLayout>

namespace
{
    QString sexToString(Sex sex)
    {
        switch (sex) {
        case Sex::MALE:
            return "Male";
        case Sex::FEMALE:
            return "Female";
        case Sex::UNKNOWN:
            return "Unknown";
        }
    }
}

AccountDetailDialog::AccountDetailDialog(QModelIndex index, AvatarManager &avatarManager, QWidget *parent)
    : QDialog(parent)
    , _avatarManager(avatarManager)
{
    setModal(true);
    setWindowTitle(tr("Account Detail Information"));

    createWidgets(index);
    setAttribute(Qt::WA_DeleteOnClose);
}

void AccountDetailDialog::createWidgets(QModelIndex index)
{
    auto username = index.siblingAtColumn(0).data().toString();
    auto firstName = index.siblingAtColumn(1).data().toString();
    auto lastName = index.siblingAtColumn(2).data().toString();
    auto sex = static_cast<Sex>(index.siblingAtColumn(3).data().toInt());
    auto country = index.siblingAtColumn(4).data().toString();
    auto language = index.siblingAtColumn(5).data().toString();
    auto birthday = index.siblingAtColumn(6).data().value<qint64>();
    auto avatarImage = _avatarManager.getAccountAvatar(firstName, lastName, sex);

    auto *layout = new QGridLayout(this);

    auto avatarLabel = new QLabel(this);
    avatarLabel->setPixmap(QPixmap::fromImage(avatarImage));
    layout->addWidget(avatarLabel, 0, 1, 1, 2);

    layout->addWidget(new QLabel(tr("Username")), 1, 0);
    layout->addWidget(new QLabel(username), 1, 1);

    layout->addWidget(new QLabel(tr("First Name")), 2, 0);
    layout->addWidget(new QLabel(firstName), 2, 1);

    layout->addWidget(new QLabel(tr("Last Name")), 3, 0);
    layout->addWidget(new QLabel(lastName), 3, 1);

    layout->addWidget(new QLabel(tr("Sex")), 4, 0);
    layout->addWidget(new QLabel(sexToString(sex)), 4, 1);

    layout->addWidget(new QLabel(tr("Country")), 5, 0);
    layout->addWidget(new QLabel(country), 5, 1);

    layout->addWidget(new QLabel(tr("Language")), 6, 0);
    layout->addWidget(new QLabel(language), 6, 1);

    layout->addWidget(new QLabel(tr("Birthday")), 7, 0);
    layout->addWidget(new QLabel(QDateTime::fromMSecsSinceEpoch(birthday).toString("dd.MM.yyyy")), 7, 1);

    setLayout(layout);
}
