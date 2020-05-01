#include "gameserver.h"

GameServer::GameServer() : QWebSocketServer(nullptr, NonSecureMode)
{
    listen(QHostAddress::Any, 8888);
    QTextStream(stdout) << "URL: " << this->serverUrl().toString() << "\n";
    connect(this, &QWebSocketServer::newConnection, this, &GameServer::connection);

    for(int i = 0; i < MAPS_NUMBER; ++i){
        _maps[i] = QSharedPointer<Map::Map>(new Map::Map(15,15));
    }
    _maps[0]->fromFile("resources/maps/map0.json");
    _maps[1]->fromFile("resources/maps/map1.json");
    _maps[2]->fromFile("resources/maps/map2.json");

    for(int i = 0; i < MAX_PLAYERS; ++i){
        _playerSocket[i] = nullptr;
        _playersReady[i] = false;
    }
}

void GameServer::connection()
{
    int newPlayerID = nextFreeId();
    if(newPlayerID >= MAX_PLAYERS){
        QTextStream(stdout) << "Player wanted to connect but max players limit has been reached yet\n";
    }
    else{
        _playerSocket[newPlayerID] = nextPendingConnection();
        sendPlayerInfo(newPlayerID);
        QTextStream(stdout) << "Player " << newPlayerID << " connected\n";

        sendMap(newPlayerID);

        connect(_playerSocket[newPlayerID], &QWebSocket::disconnected, this, [this, newPlayerID](){this->playerDisconnected(newPlayerID);});
        connect(_playerSocket[newPlayerID], &QWebSocket::binaryMessageReceived, this, &GameServer::playerRequestHandle);
    }
}

void GameServer::sendPlayerInfo(int playerID){
    QJsonObject idResponseObject;
    idResponseObject.insert("type", "playerInfo");
    idResponseObject.insert("givenID", playerID);
    _playerSocket[playerID]->sendBinaryMessage(QJsonDocument(idResponseObject).toBinaryData());
}

void GameServer::sendMap(int playerID){
    _playerSocket[playerID]->sendBinaryMessage(QJsonDocument(_maps[_currentMapId]->toJsonObject()).toBinaryData());
}

void GameServer::nextMap()
{
    ++_currentMapId;
    _currentMapId %= MAPS_NUMBER;
    _movesList.clear();
    for(int i = 0; i < MAX_PLAYERS; ++i){
        sendMap(i);
    }
    resetPlayersStats();
}

int GameServer::nextFreeId() {
    int id = -1;
    while(++id < MAX_PLAYERS && _playerSocket[id] != nullptr);
    return id;
}

void GameServer::resetPlayersStats(){
    for(int i = 0; i < MAX_PLAYERS; ++i){
        _playersReady[i] = false;
        _playersFinished[i] = false;
    }
    _playersSentFinishStatus = 0;
}

void GameServer::resetMap() {
    _movesList.clear();
    for(int i = 0; i < MAX_PLAYERS; ++i){
        sendMap(i);
    }
    resetPlayersStats();
}

void GameServer::letPlayersGo() {
    QJsonObject obj;
    obj.insert("type", "go");
    for(int i = 0; i < MAX_PLAYERS; ++i)
        _playerSocket[i]->sendBinaryMessage(QJsonDocument(obj).toBinaryData());
}

void GameServer::sendMovesList(){
    QJsonObject listObj;
    listObj.insert("type", "movelist");

    QJsonArray listArr;
    for(auto move: _movesList){
        listArr.append(move->toJsonObject());
    }

    listObj.insert("movelist", listArr);
    for(int i = 0; i < MAX_PLAYERS; ++i){
        if(_playerSocket[i] != nullptr){
            _playerSocket[i]->sendBinaryMessage(QJsonDocument(listObj).toBinaryData());
        }
    }
}

void GameServer::playerRequestHandle(const QByteArray& arr)
{
    QJsonObject objectRequest = QJsonDocument().fromBinaryData(arr).object();

    QString type = objectRequest.value("type").toString();
    if(allConnected()){
        if(type == "move"){
            _movesList.push_back(GameMove::fromJsonObject(objectRequest));
            sendMovesList();
        } else if(type == "ready"){
            _playersReady[objectRequest.value("playerID").toInt()] = true;
            if(allReady()){
                letPlayersGo();
            }
        } else if(type == "unready"){
            _playersReady[objectRequest.value("playerID").toInt()] = false;
        } else if(type == "finished" || type == "unfinished"){
            if(type == "finished")
                _playersFinished[objectRequest.value("playerID").toInt()] = true;
            else
                _playersFinished[objectRequest.value("playerID").toInt()] = false;

            ++_playersSentFinishStatus;

            if(_playersSentFinishStatus == MAX_PLAYERS){
                if(allFinished())
                    nextMap();
                else
                    resetMap();
            }
        }
    }
}

void GameServer::playerDisconnected(int playerID)
{
    QTextStream(stdout) << "Player " << playerID << " disconnected\n";
    resetPlayersStats();
    _movesList.clear();
    sendMovesList();
    _playerSocket[playerID] = nullptr;
}

bool GameServer::allConnected() const
{
    for(int i = 0; i < MAX_PLAYERS; ++i){
        if(_playerSocket[i] == nullptr)
            return false;
    }
    return true;
}

bool GameServer::allReady() const
{
    for(int i = 0; i < MAX_PLAYERS; ++i){
        if(!_playersReady[i])
            return false;
    }
    return true;
}

bool GameServer::allFinished() const
{
    for(int i = 0; i < MAX_PLAYERS; ++i){
        if(!_playersFinished[i])
            return false;
    }
    return true;
}
