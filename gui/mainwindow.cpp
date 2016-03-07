#include "mainwindow.h"

namespace gui
{
//////////////////////////////

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), state(0)
{
    setFixedSize(400, 600);

    login_edit = new QLineEdit(this);
    password_edit = new QLineEdit(this);
    password_edit->setEchoMode(QLineEdit::Password);

    login_label = new QLabel(tr("login"), this);
    password_label = new QLabel(tr("password"), this);

    login_button = new QPushButton(tr("login"), this);
    logout_botton = new QPushButton(tr("logout"), this);

    download_selected_music_button = new QPushButton(tr("download"), this);
    stop_download_button = new QPushButton(tr("stop"), this);
    select_all_button = new QPushButton(tr("select all"), this);
    clear_selection_button = new QPushButton(tr("clear\nselection"), this);

    music_list = new QListWidget(this);

    setup_widgets();
    setup_links();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setup_widgets()
{
    login_edit->setGeometry(70, 10, 60, 20);
    login_label->setGeometry(10, 10, 60, 25);
    login_button->setGeometry(10, 60, 60, 25);

    password_edit->setGeometry(70, 30, 60, 20);
    password_label->setGeometry(10, 30, 60, 25);
    logout_botton->setGeometry(75, 60, 60, 25);

    download_selected_music_button->setGeometry(10, 90, 60, 25);
    stop_download_button->setGeometry(75, 90, 60, 25);
    select_all_button->setGeometry(10, 120, 60, 25);
    clear_selection_button->setGeometry(75, 120, 60, 40);

    music_list->setGeometry(150, 0, 250, 600);
}

void MainWindow::setup_links()
{
    connect(login_button, SIGNAL(clicked(bool)), this, SLOT(login_slot()));
    connect(logout_botton, SIGNAL(clicked(bool)), this, SLOT(logout_slot()));
    connect(download_selected_music_button, SIGNAL(clicked(bool)), this, SLOT(download_audio_slot()));
    connect(stop_download_button, SIGNAL(clicked(bool)), this, SLOT(stop_download_audio_slot()));
    connect(select_all_button, SIGNAL(clicked(bool)), this, SLOT(select_all_audio_slot()));
    connect(clear_selection_button, SIGNAL(clicked(bool)), this, SLOT(clear_selected_audio_slot()));
}

void MainWindow::login_slot()
{
    if (state & (CONNECTED | AUTH_IN_PROGRESS))
        return;

    common::AuthData auth;

    auth.login = login_edit->text();
    auth.password = password_edit->text();

    emit login_signal(auth);

    state |= AUTH_IN_PROGRESS;
    state &= ~DISCONNECTED;

    login_label->setText(login_label->text().replace(" fail", ""));
}

void MainWindow::logout_slot()
{
    if (state & AUTH_IN_PROGRESS)
        return;

    if (state & CONNECTED)
    {
        emit logout_signal();
        music_list->clear();

        state &= ~CONNECTED;
        state |= DISCONNECTED;

        if (state & START)
        {
            state |= STOP;
            emit stop_download_audio_signal();
        }

        login_label->setText(login_label->text().replace(" ok", ""));
    }
}

void MainWindow::download_audio_slot()
{
    common::log(!(state & CONNECTED));
    common::log(state & START);
    if (!(state & CONNECTED) || (state & START))
        return;

    QList<QListWidgetItem*> selected_items;
    for (int i = 0; i < music_list->count(); ++i)
        if (music_list->item(i)->checkState() == Qt::Checked)
            selected_items.push_back(music_list->item(i));

    common::log(selected_items.isEmpty());
    if (selected_items.isEmpty())
        return;

    QVector<QString> item_names;
    item_names.resize(selected_items.size());
    QVector<QString>::iterator it_names = item_names.begin();

    for (QList<QListWidgetItem*>::iterator it_audio_widget = selected_items.begin(); it_audio_widget != selected_items.end(); ++it_audio_widget, ++it_names)
        (*it_names) = (*it_audio_widget)->text();

    for (it_names = item_names.begin(); it_names != item_names.end(); ++it_names)
    {
        (*it_names).replace("done ", "");
        (*it_names).replace("fail ", "");
    }

    common::log(QString("download_audio_slot"));
    emit download_audio_signal(item_names);

    state |= START;
    state &= ~STOP;
}

void MainWindow::stop_download_audio_slot()
{
    common::log(QString("stop_download_audio_slot"));
    if (!(state & CONNECTED) || (state & STOP))
        return;

    if (state & START)
    {
        state |= STOP;
        common::log(QString("send signal stop_download_audio_slot"));
        emit stop_download_audio_signal();
    }
}

void MainWindow::select_all_audio_slot()
{
    for (int i = 0; i < music_list->count(); ++i)
        music_list->item(i)->setCheckState(Qt::Checked);
}

void MainWindow::clear_selected_audio_slot()
{
    for (int i = 0; i < music_list->count(); ++i)
        music_list->item(i)->setCheckState(Qt::Unchecked);
}

void MainWindow::update_audio_list(QVector<QString> item_values)
{
    common::log(QString("update_audio_list"));
    common::log(item_values.size());
    QVector<QString>::iterator it = item_values.begin();
    for (int i = 0; i < item_values.size(); ++i, ++it)
    {
        (*it).replace("\"", "");
        (*it).replace("/", "");
        (*it).replace("|", "");
        (*it).replace("<", "");
        (*it).replace(">", "");
        (*it).replace(":", "");
        (*it).replace("?", "");
        (*it).replace("*", "");
        (*it).replace("\\", "");
        QListWidgetItem* item = new QListWidgetItem(*it, music_list);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::update_status_item_in_list(bool ok, QString item_value)
{
    common::log(QString("update_status_item_in_list"));
    common::log(item_value);
    QList<QListWidgetItem*> items = music_list->findItems(item_value, Qt::MatchContains);

    for (QList<QListWidgetItem*>::iterator it = items.begin(); it != items.end(); ++it)
        if (ok)
            (*it)->setText((*it)->text().insert(0, "done "));
    else
            (*it)->setText((*it)->text().insert(0, "fail "));
}

void MainWindow::download_fihish()
{
    common::log(QString("download_fihish"));
    state &= ~START;
    state |= STOP;
}

void MainWindow::login_state(bool ok)
{
    common::log(QString("login_state"));
    if (ok)
    {
        state |= CONNECTED;
        state &= ~AUTH_IN_PROGRESS;
        login_label->setText(login_label->text() + " ok");
    }
    else
    {
        state &= ~(CONNECTED | AUTH_IN_PROGRESS);
        state |= DISCONNECTED;
        login_label->setText(login_label->text() + " fail");;
    }
}

//////////////////////////////
}




























































































