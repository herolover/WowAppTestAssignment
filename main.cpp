#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include <memory>

namespace
{
    std::unique_ptr<QTextStream> logStream;
}

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    *logStream << type << " " << context.file << ":" << context.line << " " << context.function << ": " << msg << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir dir(QCoreApplication::applicationDirPath());
    QFile logFile(dir.absoluteFilePath("WowAppTestAssignment.log"));
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    logStream = std::make_unique<QTextStream>(&logFile);

    qInstallMessageHandler(logMessageHandler);

    MainWindow w;
    w.show();
    return a.exec();
}
