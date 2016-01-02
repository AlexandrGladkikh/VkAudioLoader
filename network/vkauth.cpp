#include "vkauth.h"

namespace vk_api
{
//////////////////////////////

VkAuth::VkAuth(QWidget *parent) : QWidget(parent), step(0)
{
    web_view = new QWebView(this);
    web_view->setVisible(false);
    QObject::connect(web_view, SIGNAL(loadFinished(bool)),this , SLOT(check_load_page(bool)));
}

void VkAuth::login(common::AuthData auth)
{
    common::log(QString("login"));
    auth_data = auth;
    web_view->load(QUrl("https://oauth.vk.com/authorize?client_id=5158383&scope=audio&redirect_uri=https://oauth.vk.com/blank.html&display=popup&v=5.42&response_type=token&revoke=1"));
}

void VkAuth::check_load_page(bool ok)
{
    common::log((ok ? "ok" : "fail"));
    switch (step)
    {
    case LOGIN_STEP: set_auth_field(); break;
    case AGREE_STEP: agree_on_access(); break;
    case GET_ACCESS_TOKEN_STEP: get_access_token(); break;
    }
}

void VkAuth::set_auth_field()
{
    common::log("set_auth_field");
    common::log(auth_data.login);
    common::log(auth_data.password);
    QWebFrame* frame_login = web_view->page()->mainFrame();
    QWebElement document_login = frame_login->documentElement();
    QWebElement search_login = document_login.findFirst("input[name=email]");
    search_login.setAttribute("value", auth_data.login);

    QWebFrame* frame_password = web_view->page()->mainFrame();
    QWebElement document_password = frame_password->documentElement();
    QWebElement search_password = document_password.findFirst("input[name=pass]");
    search_password.setAttribute("value", auth_data.password);

    QWebFrame* frame_buttom = web_view->page()->mainFrame();
    QWebElement document_buttom = frame_buttom->documentElement();
    QWebElement search_buttom = document_buttom.findFirst("input[name=submit]");
    search_buttom.evaluateJavaScript("this.click()");

    QUrl url = web_view->url();
    common::log(url.toString());
    step++;
}

void VkAuth::agree_on_access()
{
    common::log("agree_on_access");
    QWebFrame* frame_buttom = web_view->page()->mainFrame();
    QWebElement document_buttom = frame_buttom->documentElement();
    QWebElement search_buttom = document_buttom.findFirst("button[id=install_allow]");
    search_buttom.evaluateJavaScript("this.click()");

    QUrl url = web_view->url();
    common::log(url.toString());
    step++;
}

void VkAuth::get_access_token()
{
    common::log("get_access_token");
    QUrl url = web_view->url();
    common::log(url.toString());
    QUrlQuery url_query(url.toString().replace("#", "&"));

    common::OAuthData oauth_data;
    oauth_data.access_token = url_query.queryItemValue("access_token");
    oauth_data.user_id = url_query.queryItemValue("user_id");
    common::log(oauth_data.access_token);

    emit login_state((oauth_data.access_token.isEmpty() ? false : true), oauth_data);

    step = 0;
}

//////////////////////////////
}



























