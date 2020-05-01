#include "gamewindow.h"

#include "q_includes.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow w;
    w.setMinimumSize(1000, 600);

    w.show();
    return a.exec();
}
