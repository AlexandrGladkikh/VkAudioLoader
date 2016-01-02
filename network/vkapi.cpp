#include "vkapi.h"

namespace vk_api
{
//////////////////////////////

VkApi::VkApi(QWidget *parent) : QWidget(parent)
{
    vk_auth = new VkAuth(this);
    vk_audio = new VkAudio(oauth_data, this);

    QObject::connect(vk_auth, SIGNAL(login_state(bool, common::OAuthData)), this, SLOT(login_state_slot(bool, common::OAuthData)));
    QObject::connect(vk_audio, SIGNAL(audio_list_signal(QVector<QString>)), this, SIGNAL(download_audio_list_signal(QVector<QString>)));
    QObject::connect(vk_audio, SIGNAL(download_audio_item_state(bool, QString)), this, SIGNAL(download_audio_item_state_signal(bool, QString)));
    QObject::connect(vk_audio, SIGNAL(download_finished()), this, SIGNAL(download_finished()));
}

VkApi::~VkApi()
{

}

void VkApi::login_slot(common::AuthData auth)
{
    common::log("login_slot");
    vk_auth->login(auth);
}

void VkApi::logout_slot()
{
    oauth_data.access_token = "";
    oauth_data.user_id = "";

    vk_audio->stop_download_audio();
}

void VkApi::download_audio_slot(QVector<QString> audio_list)
{
    vk_audio->download_audio(audio_list);
}

void VkApi::stop_slot()
{
    vk_audio->stop_download_audio();
}

void VkApi::login_state_slot(bool ok, common::OAuthData _oauth_data)
{
    oauth_data = _oauth_data;
    if (ok)
        vk_audio->get_audio_list();

    emit login_state_signal(ok);
}

//////////////////////////////
}

























