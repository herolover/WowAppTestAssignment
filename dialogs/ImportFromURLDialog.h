#pragma once

#include <QDialog>

#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressBar>
#include <QPushButton>

class ImportFromURLDialog: public QDialog
{
    Q_OBJECT

    Q_PROPERTY(bool isDownloading READ isDownloading WRITE setIsDownloading NOTIFY isDownloadingChanged)

public:
    ImportFromURLDialog(QWidget *parent = nullptr);

    bool isDownloading() const;
    void setIsDownloading(bool isDownloading);

signals:
    void isDownloadingChanged(bool isDownloading);

private:
    void createWidgets();
    void createLayout();

    void onImport();

    QLineEdit *_urlEdit;
    QPushButton *_importButton;
    QProgressBar *_progressBar;
    QNetworkAccessManager *_networkManager;
    QNetworkReply *_currentReply;

    bool _isDownloading;
};


