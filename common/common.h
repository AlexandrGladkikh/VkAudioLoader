#ifndef COMMON
#define COMMON

#include <QString>

enum
{
    CONNECTED       = 0x0001,
    DISCONNECTED    = 0x0002,
    PAUSE           = 0x0004,
    STOP            = 0x0008,
    START           = 0x0010,
};

struct AuthData
{
    QString login;
    QString password;
};

#endif // COMMON

