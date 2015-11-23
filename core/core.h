#ifndef CORE
#define CORE

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "../gui/mainwindow.h"

class Core : public QWidget
{
    Q_OBJECT

private:
    QNetworkAccessManager* networkManager;
    MainWindow* mainWindow;

    QString accessToken;

    int state;

public:
    Core(QWidget* parent = 0);
    ~Core();

private slots:
    void GetData(QNetworkReply * reply);

public slots:
    void Login(AuthData);
    void Logout();

    void Start();
    void Stop();
    void Pause();

    void GetSelectedItem(QVector<QString> itemValues);

signals:
    void SendItems(QVector<QString>);
    void SendStateitem(QString itemValue);

};

#endif // CORE

