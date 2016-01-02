#ifndef NETWORK
#define NETWORK

#include <QWidget>

#include "vkauth.h"
#include "vkaudio.h"
#include "../common/common.h"

namespace vk_api
{
//////////////////////////////

class VkApi : public QWidget
{
    Q_OBJECT

private:
    VkAuth* vk_auth;
    VkAudio* vk_audio;

    common::OAuthData oauth_data;

public:
    VkApi(QWidget* parent = 0);
    ~VkApi();

public slots:
    void login_slot(common::AuthData auth);
    void logout_slot();
    void download_audio_slot(QVector<QString> audio_list);
    void stop_slot();

private slots:
    void login_state_slot(bool ok, common::OAuthData _oauth_data);

signals:
    void login_state_signal(bool);

    void download_audio_list_signal(QVector<QString>);
    void download_audio_item_state_signal(bool, QString);

    void download_finished();
};

//////////////////////////////
}

#endif // NETWORK

