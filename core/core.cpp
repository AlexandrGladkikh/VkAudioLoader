#include "core.h"

Core::Core(QWidget *parent) : QWidget(parent)
{
    networkManager = new QNetworkAccessManager(this);
    mainWindow = new MainWindow(this);

    connect(mainWindow, SIGNAL(SendAuthData(AuthData)), this, SLOT(Login(AuthData)));
    connect(mainWindow, SIGNAL(Disconnect()), this, SLOT(Logout()));
    connect(mainWindow, SIGNAL(SendSelectedItems(QVector<QString>)), this, SLOT(GetSelectedItem(QVector<QString>)));
    connect(mainWindow, SIGNAL(Start()), this, SLOT(Start()));
    connect(mainWindow, SIGNAL(Stop()), this, SLOT(Stop()));
    connect(mainWindow, SIGNAL(Pause()), this, SLOT(Pause()));

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(GetData(QNetworkReply*)));

    connect(this, SIGNAL(SendItems(QVector<QString>)), mainWindow, SLOT(AddItems(QVector<QString>)));
    connect(this, SIGNAL(SendStateitem(QString)), mainWindow, SLOT(SetStateItem(QString)));
}

Core::~Core()
{

}

void Core::GetData(QNetworkReply *reply)
{
    QString response = reply->readAll();

#ifdef QT_DEBUG
    qDebug() << "loginForm" << response;
#endif

    QString signatureStart = "<form method=";
    QString signatureEnd = "</form>";

    size_t signatureStartPos = response.indexOf(signatureStart);
    size_t signatureEndPos = response.indexOf(signatureEnd) - signatureStartPos + signatureEnd.length();

    if (signatureStartPos == -1 && signatureEndPos == -1)
        return;

    QString loginForm = response.mid(signatureStartPos, signatureEndPos);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(QUrl("https://login.vk.com/?act=login&amp;soft=1&amp;utf8=1"));
    QByteArray requestBody;

    networkManager->post(request, requestBody);

#ifdef QT_DEBUG
    qDebug() << "loginForm" << loginForm << signatureStartPos << signatureEndPos;
#endif
}

void Core::Login(AuthData)
{
#ifdef QT_DEBUG
    qDebug() << "Login";
#endif

networkManager->get(QNetworkRequest(QUrl("https://oauth.vk.com/authorize?client_id=5158383&scope=audio&redirect_uri=https://oauth.vk.com/blank.html&display=wap&v=5.0&response_type=token&revoke=1")));
}

void Core::Logout()
{
#ifdef QT_DEBUG
    qDebug() << "Logout";
#endif
}

void Core::Start()
{
#ifdef QT_DEBUG
    qDebug() << "Start";
#endif
}

void Core::Stop()
{
#ifdef QT_DEBUG
    qDebug() << "Stop";
#endif
}

void Core::Pause()
{
#ifdef QT_DEBUG
    qDebug() << "Pause";
#endif
}

void Core::GetSelectedItem(QVector<QString> itemValues)
{
#ifdef QT_DEBUG
    qDebug() << "GetSelectedItem";
#endif
}










































