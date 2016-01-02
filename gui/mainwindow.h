#ifndef MAINWINDOW
#define MAINWINDOW

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QList>
#include <QVector>

#include "common/common.h"

namespace gui
{
//////////////////////////////

enum
{
    CONNECTED                   = 0x0001,
    AUTH_IN_PROGRESS            = 0x0002,
    DISCONNECTED                = 0x0004,
    DISCONNECTED_IN_PROGRESS    = 0x0008,
    STOP           = 0x0010,
    START           = 0x0020,
};

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QLineEdit* login_edit;
    QLineEdit* password_edit;
    QLabel* login_label;
    QLabel* password_label;

    QPushButton* login_button;
    QPushButton* logout_botton;

    QPushButton* download_selected_music_button;
    QPushButton* stop_download_button;
    QPushButton* select_all_button;
    QPushButton* clear_selection_button;

    QListWidget* music_list;

    int state;

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void setup_widgets();
    void setup_links();

private slots:
    void login_slot();
    void logout_slot();

    void download_audio_slot();
    void stop_download_audio_slot();

    void select_all_audio_slot();
    void clear_selected_audio_slot();

public slots:
    void update_audio_list(QVector<QString> item_values);
    void update_status_item_in_list(bool ok, QString item_value);
    void download_fihish();
    void login_state(bool);

signals:
    void login_signal(common::AuthData);
    void logout_signal();

    void download_audio_signal(QVector<QString>);
    void stop_download_audio_signal();
};

//////////////////////////////
}

#endif // MAINWINDOW




















































