#ifndef COMMON
#define COMMON

#include <QString>
#include <QVariant>

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace common
{
//////////////////////////////

struct AuthData
{
    QString login;
    QString password;
};

struct OAuthData
{
    QString access_token;
    QString user_id;
};

void log(QVariant msg_log);

size_t get_value_from_string(QString &source, QString& target, QString left_border, QString right_border, size_t start_from);

//////////////////////////////
}

#endif // COMMON

