#include "core.h"

namespace core
{
//////////////////////////////

Core::Core(QWidget *parent) : QWidget(parent)
{
    setFixedSize(400, 600);

    vk_api = new vk_api::VkApi(this);
    main_window = new gui::MainWindow(this);

    connect(vk_api, SIGNAL(login_state_signal(bool)), main_window, SLOT(login_state(bool)));

    connect(main_window, SIGNAL(login_signal(common::AuthData)), vk_api, SLOT(login_slot(common::AuthData)));
    connect(main_window, SIGNAL(logout_signal()), vk_api, SLOT(logout_slot()));
    connect(main_window, SIGNAL(download_audio_signal(QVector<QString>)), vk_api, SLOT(download_audio_slot(QVector<QString>)));
    connect(main_window, SIGNAL(stop_download_audio_signal()), vk_api, SLOT(stop_slot()));

    connect(vk_api, SIGNAL(download_audio_list_signal(QVector<QString>)), main_window, SLOT(update_audio_list(QVector<QString>)));
    connect(vk_api, SIGNAL(download_audio_item_state_signal(bool, QString)), main_window, SLOT(update_status_item_in_list(bool, QString)));
    connect(vk_api, SIGNAL(download_finished()), main_window, SLOT(download_fihish()));
}

Core::~Core()
{

}

//////////////////////////////
}










































