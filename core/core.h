#ifndef CORE
#define CORE

#include <QWidget>
#include "network/vkapi.h"

#include "../gui/mainwindow.h"
#include "common/common.h"

namespace core
{
//////////////////////////////

class Core : public QWidget
{
    Q_OBJECT

private:
    gui::MainWindow* main_window;
    vk_api::VkApi* vk_api;

public:
    Core(QWidget* parent = 0);
    ~Core();
};

//////////////////////////////
}

#endif // CORE

