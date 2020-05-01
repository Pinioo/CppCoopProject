#include <QCoreApplication>
#include <QSharedPointer>

#include <gameserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new GameServer();
    return a.exec();
}
