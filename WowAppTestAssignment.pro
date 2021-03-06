QT += core gui widgets network sql concurrent

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AvatarManager.cpp \
    Database.cpp \
    FileDownloader.cpp \
    FileImporter.cpp \
    MainWindow.cpp \
    delegates/GroupAccountTreeViewDelegate.cpp \
    dialogs/AccountDetailDialog.cpp \
    dialogs/ImportFromFileDialog.cpp \
    dialogs/ImportFromURLDialog.cpp \
    main.cpp \
    models/AccountListModel.cpp \
    models/GroupAccountTreeModel.cpp \
    widgets/FileImporterWidget.cpp \
    widgets/GroupAccountTreeViewWidget.cpp

HEADERS += \
    AvatarManager.h \
    Database.h \
    FileDownloader.h \
    FileImporter.h \
    MainWindow.h \
    delegates/GroupAccountTreeViewDelegate.h \
    dialogs/AccountDetailDialog.h \
    dialogs/ImportFromFileDialog.h \
    dialogs/ImportFromURLDialog.h \
    models/Account.h \
    models/AccountListModel.h \
    models/Group.h \
    models/GroupAccountTreeModel.h \
    models/GroupId.h \
    models/Sex.h \
    widgets/FileImporterWidget.h \
    widgets/GroupAccountTreeViewWidget.h

FORMS +=

TRANSLATIONS += \
    WowAppTestAssignment_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
