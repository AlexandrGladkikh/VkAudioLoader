#ifndef VKGETAUDIOLIST
#define VKGETAUDIOLIST

#include <QWidget>
#include <QNetworkAccessManager>
#include <QMap>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QStringList>

#include "common/common.h"

namespace vk_api
{
//////////////////////////////

enum
{
    GET_AUDIO_LIST = 0,
    DOWNLOAD_AUDIO_ITEMS
};

struct AudioLinks
{
    QMap<QString, QString> audio_links;
    QVector<QString> need_load_audio;

    int next_audio_index;

    AudioLinks();

    QString get_next_link();
};

size_t get_audio_info(QString &source, QString& audio_name, QString& link, size_t start_pos);

class VkAudio : public QWidget
{
    Q_OBJECT

private:
    QNetworkAccessManager* network_manager;

    common::OAuthData& oauth_data;

    int current_action;

    QStringList save_path;

    AudioLinks audio_links;

    bool stop;

    bool create_dir;

private:
    void source_audio_list_handler(QString &source);

    void save_audio_item(QNetworkReply *audio_data);
    void get_next_audio();

public:
    VkAudio(common::OAuthData& oauth_data, QWidget* parent = 0);
    ~VkAudio();

    void get_audio_list();

    void download_audio(QVector<QString> &audio_list);
    void stop_download_audio();

private slots:
    void reply_finished(QNetworkReply *reply);

signals:
    void audio_list_signal(QVector<QString>);
    void download_audio_item_state(bool, QString);
    void download_finished();
};

//////////////////////////////
}

#endif // VKGETAUDIOLIST

