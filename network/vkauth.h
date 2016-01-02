#ifndef VKAUTH
#define VKAUTH

#include <QWidget>
#include <QUrl>
#include <QtWebKitWidgets/QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QUrlQuery>

#include "common/common.h"

namespace vk_api
{
//////////////////////////////

enum Step
{
    LOGIN_STEP = 0,
    AGREE_STEP,
    GET_ACCESS_TOKEN_STEP
};

class VkAuth : public QWidget
{
    Q_OBJECT

private:
    QWebView* web_view;

    common::AuthData auth_data;
    int step;

public:
    VkAuth(QWidget* parent = 0);

    void login(common::AuthData auth);

private:
    void set_auth_field();
    void agree_on_access();
    void get_access_token();

public slots:
    void check_load_page(bool ok);

signals:
    void login_state(bool, common::OAuthData oauth_data);
};

//////////////////////////////
}

#endif // VKAUTH

