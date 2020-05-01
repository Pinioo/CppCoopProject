#ifndef GAME_H
#define GAME_H

class Game;

#include "mapdrawing.h"
#include "mapmovablecharacter.h"
#include "gamewindow.h"
#include "gamemove.h"
#include "q_includes.h"
#include "moveslist.h"
#include "instanceof.h"
#include "mapfinish.h"

class Game : public QObject
{
    Q_OBJECT

private:
    QWebSocket _mySocket;

    QSharedPointer<Map::Map> _map;

    QSharedPointer<Map::MapMovableCharacter> _players[2];

    MovesList _movesList;

    void proceedMoves();
    void sendFinishedStatus();

    void clearMovesList();

    bool _ready{false};
    int _playerID{-1};

public:
    Game();

    QSharedPointer<Map::Map> map() {return _map;}

    void requestStepForward(int);
    void requestRightTurn(int);
    void requestLeftTurn(int);

    void connectToServer(QString url);

    MapDrawing* mapDraw;

signals:
    void sigDisconnect();
    void sigConnect();
    void mapChanged();
    void sigReady(bool);
    void movesListChanged(QStringList);
    void playersMoving();
    void newMap(QColor);

public slots:
    void disconnected();
    void connected();
    void newData(const QByteArray&);
    void ready();
};

#endif // GAME_H
