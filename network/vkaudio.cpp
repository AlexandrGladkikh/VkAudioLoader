#include "vkaudio.h"

namespace vk_api
{
//////////////////////////////

AudioLinks::AudioLinks() : next_audio_index(0)
{

}

QString AudioLinks::get_next_link()
{
    if (!(next_audio_index < need_load_audio.size()))
    {
        next_audio_index = 0;
        return QString("");
    }

    QMap<QString, QString>::iterator it = audio_links.find(need_load_audio[next_audio_index++]);

    if (it != audio_links.end())
        return it.value();
    else
    {
        next_audio_index = 0;
        return QString("");
    }
}

size_t get_audio_info(QString &source, QString& audio_name, QString& link, size_t start_pos)
{
    QString artist;
    QString title;
    size_t current_pos;

    if ((current_pos = common::get_value_from_string(source, artist, "artist\":\"", "\",", start_pos)) == (size_t)-1)
        return current_pos;
    if ((current_pos = common::get_value_from_string(source, title, "title\":\"", "\",", current_pos)) == (size_t)-1)
        return current_pos;

    audio_name = artist + "-" + title;

    if ((current_pos = common::get_value_from_string(source, link, "url\":\"", "\"", current_pos)) == (size_t)-1)
        return current_pos;

    link.replace("\\", "");

    return current_pos;
}

VkAudio::VkAudio(common::OAuthData &oauth_data, QWidget *parent) : QWidget(parent), oauth_data(oauth_data), current_action(0), stop(false)
{
    network_manager = new QNetworkAccessManager(this);

    save_path = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);

    QObject::connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(reply_finished(QNetworkReply*)));
}

VkAudio::~VkAudio()
{

}

void VkAudio::source_audio_list_handler(QString &source)
{
    common::log(QString("source_audio_list_handler"));
    QString audio_name;
    QString audio_link;
    size_t start_pos = 0;

    QVector<QString> audio_list;

    while ((start_pos = get_audio_info(source, audio_name, audio_link, start_pos)) != (size_t)-1)
    {
        audio_name.replace("\"", "");
        audio_name.replace("/", "");
        audio_name.replace("|", "");
        audio_name.replace("<", "");
        audio_name.replace(">", "");
        audio_name.replace(":", "");
        audio_name.replace("?", "");
        audio_name.replace("*", "");
        audio_name.replace("\\", "");
        audio_links.audio_links[audio_name] = audio_link;
        audio_list.push_back(audio_name);
    }

    emit audio_list_signal(audio_list);
}

void VkAudio::save_audio_item(QNetworkReply* audio_data)
{
    common::log(QString("save_audio_item"));

    QDir dir;

    dir.mkdir(save_path.at(0) + "/VkMusic/");

    common::log(audio_links.need_load_audio[audio_links.next_audio_index - 1]);
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace("\"", "");
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace("/", "");
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace("|", "");
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace("<", "");
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace(">", "");
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace(":", "");
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace("?", "");
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace("*", "");
    audio_links.need_load_audio[audio_links.next_audio_index - 1].replace("\\", "");
    common::log(save_path.at(0) + "/VkMusic/" + audio_links.need_load_audio[audio_links.next_audio_index - 1].replace("\"", "") + ".mp3");
    QFile file(save_path.at(0) + "/VkMusic/" + audio_links.need_load_audio[audio_links.next_audio_index - 1] + ".mp3");

    bool result_work_with_file;
    if (!(result_work_with_file = file.open(QIODevice::WriteOnly)))
    {
        common::log(QString("if (!file.open(QIODevice::WriteOnly))"));
        emit download_audio_item_state(false, audio_links.need_load_audio[audio_links.next_audio_index - 1]);
    }

    if (result_work_with_file)
    {
        if (file.write(audio_data->readAll()) != -1)
            emit download_audio_item_state(true, audio_links.need_load_audio[audio_links.next_audio_index - 1]);
        else
            emit download_audio_item_state(false, audio_links.need_load_audio[audio_links.next_audio_index - 1]);

        file.close();
    }

    if (!stop)
    {
        get_next_audio();
    }
}

void VkAudio::get_next_audio()
{
    common::log(QString("get_next_audio"));

    QString url(audio_links.get_next_link());
    if (!url.isEmpty())
        network_manager->get(QNetworkRequest(QUrl(url)));
    else
    {
        common::log(QString("if (!url.isEmpty()) else"));
        emit download_finished();
    }
}

void VkAudio::reply_finished(QNetworkReply* reply)
{
    common::log(QString("reply_finished"));
    int error = reply->error();
    if (error)
    {
        common::log(QString("error"));
        common::log(error);
        if (current_action == DOWNLOAD_AUDIO_ITEMS)
            get_next_audio();

        return;
    }

    if (current_action == GET_AUDIO_LIST)
    {
        QString reply_data(QByteArray(reply->readAll()).constData());
        source_audio_list_handler(reply_data);
    }
    else if (current_action == DOWNLOAD_AUDIO_ITEMS)
    {
        save_audio_item(reply);
    }

    reply->deleteLater();
}

void VkAudio::get_audio_list()
{
    current_action = GET_AUDIO_LIST;

    QUrlQuery url_query;

    url_query.addQueryItem("user_id", oauth_data.user_id);
    url_query.addQueryItem("version", "5.42");
    url_query.addQueryItem("access_token", oauth_data.access_token);

    common::log(QString("https://api.vk.com/method/audio.get?" + url_query.toString()));
    network_manager->get(QNetworkRequest(QUrl("https://api.vk.com/method/audio.get?" + url_query.toString())));

    common::log(QString("get_audio_list"));
}

void VkAudio::download_audio(QVector<QString> &audio_list)
{
    common::log(QString("download_audio"));
    current_action = DOWNLOAD_AUDIO_ITEMS;
    audio_links.need_load_audio = audio_list;
    stop = false;

    get_next_audio();
}

void VkAudio::stop_download_audio()
{
    stop = true;
}

//////////////////////////////
}
