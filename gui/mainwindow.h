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

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QLineEdit* loginEdit;
    QLineEdit* passwordEdit;
    QLabel* loginLabel;
    QLabel* passwordLabel;

    QPushButton* loginButton;
    QPushButton* logoutBotton;

    QPushButton* downloadSelectedMusicButton;
    QPushButton* pauseDownloadButton;
    QPushButton* selectAllButton;
    QPushButton* clearSelectionButton;

    QListWidget* musicList;

    int state;

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void SetupWidgets();
    void SetupLinks();

private slots:
    void Login();
    void Logout();

    void DownloadSelectedMusic();
    void PauseDownload();

    void SelectAll();
    void ClearSelected();

public slots:
    void AddItems(QVector<QString> itemValues);
    void SetStateItem(QString itemValue);

signals:
    void SendAuthData(AuthData);
    void Disconnect();

    void SendSelectedItems(QVector<QString>);

    void Start();
    void Stop();
    void Pause();
};

#endif // MAINWINDOW




















































