#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), state(false)
{
    setFixedSize(400, 600);

    loginEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginLabel = new QLabel("login", this);
    passwordLabel = new QLabel("password", this);

    loginButton = new QPushButton("login", this);
    logoutBotton = new QPushButton("logout", this);

    downloadSelectedMusicButton = new QPushButton("download", this);
    pauseDownloadButton = new QPushButton("pause", this);
    selectAllButton = new QPushButton("select all", this);
    clearSelectionButton = new QPushButton("clear\nselection", this);

    musicList = new QListWidget(this);

    SetupWidgets();
    SetupLinks();
}

MainWindow::~MainWindow()
{

}

void MainWindow::SetupWidgets()
{
    loginEdit->setGeometry(70, 10, 60, 20);
    loginLabel->setGeometry(10, 10, 60, 25);
    loginButton->setGeometry(10, 60, 60, 25);

    passwordEdit->setGeometry(70, 30, 60, 20);
    passwordLabel->setGeometry(10, 30, 60, 25);
    logoutBotton->setGeometry(75, 60, 60, 25);

    downloadSelectedMusicButton->setGeometry(10, 90, 60, 25);
    pauseDownloadButton->setGeometry(75, 90, 60, 25);
    selectAllButton->setGeometry(10, 120, 60, 25);
    clearSelectionButton->setGeometry(75, 120, 60, 40);

    musicList->setGeometry(150, 0, 250, 600);
}

void MainWindow::SetupLinks()
{
    connect(loginButton, SIGNAL(clicked(bool)), this, SLOT(Login()));
    connect(logoutBotton, SIGNAL(clicked(bool)), this, SLOT(Logout()));
    connect(downloadSelectedMusicButton, SIGNAL(clicked(bool)), this, SLOT(DownloadSelectedMusic()));
    connect(pauseDownloadButton, SIGNAL(clicked(bool)), this, SLOT(PauseDownload()));
    connect(selectAllButton, SIGNAL(clicked(bool)), this, SLOT(SelectAll()));
    connect(clearSelectionButton, SIGNAL(clicked(bool)), this, SLOT(ClearSelected()));
}

void MainWindow::Login()
{
    if (state & CONNECTED)
        return;

    AuthData auth;

    auth.login = loginEdit->text();
    auth.password = passwordEdit->text();

    emit SendAuthData(auth);

    state |= CONNECTED;
    state |= ~DISCONNECTED;
}

void MainWindow::Logout()
{
    if (state & CONNECTED)
    {
        emit Stop();
        emit Disconnect();

        state &= ~CONNECTED;
        state |= DISCONNECTED;
    }
}

void MainWindow::DownloadSelectedMusic()
{
    if (!(state & CONNECTED))
        return;

    if (state & PAUSE)
    {
        state |= START;
        emit Start();
        return;
    }
    else if (state & START)
        return;

    QList<QListWidgetItem*> selectedItems = musicList->selectedItems();

    if (selectedItems.isEmpty())
        return;

    QVector<QString> itemNames;
    itemNames.resize(selectedItems.size());
    QVector<QString>::iterator itNames = itemNames.begin();

    for (QList<QListWidgetItem*>::iterator itWidgets = selectedItems.begin(); itWidgets != selectedItems.end(); ++itWidgets, ++itNames)
        (*itNames) = (*itWidgets)->text();

    emit SendSelectedItems(itemNames);
    emit Start();

    state |= START;
}

void MainWindow::PauseDownload()
{
    if (!(state & CONNECTED))
        return;

    if (state & START)
    {
        state |= ~START;
        state |= PAUSE;
        emit Pause();
    }
}

void MainWindow::SelectAll()
{
    for (int i = 0; i < musicList->count(); ++i)
        musicList->item(i)->setCheckState(Qt::Checked);
}

void MainWindow::ClearSelected()
{
    for (int i = 0; i < musicList->count(); ++i)
        musicList->item(i)->setCheckState(Qt::Unchecked);
}

void MainWindow::AddItems(QVector<QString> itemValues)
{
    QVector<QString>::iterator it = itemValues.begin();
    for (int i = 0; i < itemValues.size(); ++i, ++it)
    {
        QListWidgetItem* item = new QListWidgetItem(*it, musicList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::SetStateItem(QString itemValue)
{
    QList<QListWidgetItem*> items = musicList->findItems(itemValue, Qt::MatchFixedString);

    for (QList<QListWidgetItem*>::iterator it = items.begin(); it != items.end(); ++it)
        (*it)->text() = (*it)->text() + " done";
}






























































































