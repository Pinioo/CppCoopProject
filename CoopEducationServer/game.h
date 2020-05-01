#ifndef GAME_H
#define GAME_H

#include "mapmovablecharacter.h"

class Game : public QObject
{
    Q_OBJECT
enum UserMode{
    HOST, GUEST
};

enum SingleAction{
    MOVE, LEFT, RIGHT, WAIT
};

class GameAction {
public:
    GameAction(UserMode um, SingleAction sa) : uMode(um), action(sa){};
    const UserMode uMode;
    const SingleAction action;
};

private:
    UserMode myMode;

    QTcpSocket _mySocket;

    QSharedPointer<Map::Map> _map;

    QSharedPointer<Map::MapMovableCharacter> _players[2];

    int _playerID{-1};


public:
    Game();

    QSharedPointer<Map::Map> map() {return _map;}

    void requestStepForward(int);
    void requestRightTurn(int);
    void requestLeftTurn(int);

    bool connectToServer(QString ipAddress, int port);

signals:
    void sigDisconnect();
    void sigConnect();

public slots:
    void disconnected();
    void connected();
};

#endif // GAME_H
