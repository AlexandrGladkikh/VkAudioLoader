#include "common.h"

namespace common
{
//////////////////////////////

void log(QVariant msg_log)
{
#ifdef QT_DEBUG
    qDebug() << msg_log;
#endif
}

size_t get_value_from_string(QString &source, QString& target, QString left_border, QString right_border, size_t start_from)
{
    size_t start_pos = source.indexOf(left_border, start_from);

    if (start_pos == (size_t)-1)
        return start_pos;

    size_t end_pos = source.indexOf(right_border, start_pos + left_border.length());

    if (end_pos == (size_t)-1)
        return end_pos;

    target = source.mid(start_pos + left_border.length(), end_pos - (start_pos + left_border.length()));

    return end_pos;
}

//////////////////////////////
}
