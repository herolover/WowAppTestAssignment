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
    Database.cpp \
    FileDownloader.cpp \
    FileImporter.cpp \
    MainWindow.cpp \
    delegates/AccountListViewDelegate.cpp \
    dialogs/ImportFromURLDialog.cpp \
    main.cpp \
    models/GroupTreeModel.cpp \
    widgets/FileImporterWidget.cpp

HEADERS += \
    Database.h \
    FileDownloader.h \
    FileImporter.h \
    FileImporterWidget.h \
    MainWindow.h \
    delegates/AccountListViewDelegate.h \
    dialogs/ImportFromURLDialog.h \
    models/Account.h \
    models/Group.h \
    models/GroupId.h \
    models/GroupTreeModel.h \
    models/Sex.h \
    widgets/FileImporterWidget.h

FORMS +=

TRANSLATIONS += \
    WowAppTestAssignment_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
