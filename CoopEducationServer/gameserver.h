#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

#include "gamemove.h"
#include "map.h"

class GameServer : public QWebSocketServer
{
    Q_OBJECT
private:
    static constexpr int MAPS_NUMBER = 3;
    static constexpr int MAX_PLAYERS = 2;
    static constexpr int DEFAULT_PORT = 8888;

    QSharedPointer<Map::Map> _maps[MAPS_NUMBER];
    QWebSocket* _playerSocket[MAX_PLAYERS];

    QVector<GameMove*> _movesList;
    int _currentMapId{0};
    int _playersSentFinishStatus{0};
    bool _playersReady[MAX_PLAYERS];
    bool _playersFinished[MAX_PLAYERS];

    void nextMap();

    void sendMap(int);
    qint64 sendPlayerInfo(int);
    void sendMovesList();

    void resetPlayersStats();
    void resetMap();

    void letPlayersGo();

    int nextFreeId();

private slots:
    void playerRequestHandle(const QByteArray&);
    void playerDisconnected(int);
    void connection();

public:
    GameServer();
    bool allConnected() const;
    bool allReady() const;
    bool allFinished() const;
};

#endif // GAMESERVER_H
