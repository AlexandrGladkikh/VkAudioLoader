#include <QApplication>

#include "core/core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Core w;
    w.show();



    return a.exec();
}
