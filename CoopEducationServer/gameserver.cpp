#include "gameserver.h"
#include <exception>

GameServer::GameServer() : QWebSocketServer(nullptr, NonSecureMode)
{
    listen(QHostAddress::Any, 8888);
    if(this->serverPort() == 0){
        listen(QHostAddress::Any);
    }
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

//-------------------SENDING MESSAGES TO PLAYERS---------------------
// SEND TO NEWLY CONNECTED PLAYER HIS ID
qint64 GameServer::sendPlayerInfo(int playerID){
    QJsonObject idResponseObject;
    idResponseObject.insert("type", "playerInfo");
    idResponseObject.insert("givenID", playerID);
    return _playerSocket[playerID]->sendBinaryMessage(QJsonDocument(idResponseObject).toBinaryData());
}

// SEND CURRENT MAP TO PLAYER
void GameServer::sendMap(int playerID){
    if(_playerSocket[playerID]->sendBinaryMessage(QJsonDocument(_maps[_currentMapId]->toJsonObject()).toBinaryData()) > 0){
        QTextStream(stdout) << "[Player " << playerID << "] MAP " << _currentMapId << " SENT\n";
    }
    else{
        QTextStream(stdout) << "[Player " << playerID << "] UNREACHABLE\n";
        _playerSocket[playerID]->close();
    }
}

// SEND CURRENT MOVES LIST TO PLAYER
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
            if(_playerSocket[i]->sendBinaryMessage(QJsonDocument(listObj).toBinaryData())){
                QTextStream(stdout) << "[Player " << i << "] MOVES LIST SENT\n";
            }
            else{
                QTextStream(stdout) << "[Player " << i << "] UNREACHABLE\n";
                _playerSocket[i]->close();
            }
        }
    }
}

// SEND TO ALL PLAYERS 'GO' COMMAND
void GameServer::letPlayersGo() {
    QJsonObject obj;
    obj.insert("type", "go");
    for(int i = 0; i < MAX_PLAYERS; ++i){
        if(_playerSocket[i]->sendBinaryMessage(QJsonDocument(obj).toBinaryData()))
            QTextStream(stdout) << "[Player " << i << "] GO SIGNAL SENT\n";
        else{
            QTextStream(stdout) << "[Player " << i << "] UNREACHABLE\n";
            _playerSocket[i]->close();
        }
    }
}

//------------------------------------------------------------
//-----------------RESETING PLAYERS' STATS--------------------

void GameServer::resetPlayersStats(){
    for(int i = 0; i < MAX_PLAYERS; ++i){
        _playersReady[i] = false;
        _playersFinished[i] = false;
    }
    _playersSentFinishStatus = 0;
    QTextStream(stdout) << "[All Players] STATUS RESET\n";
}

void GameServer::resetMap() {
    _movesList.clear();
    for(int i = 0; i < MAX_PLAYERS; ++i){
        sendMap(i);
    }
    resetPlayersStats();
}

//------------------------------------------------------------------
//------------------HANDLING WEB SOCKETS EVENTS---------------------
// NEW PLAYER WANTS TO CONNECT

void GameServer::connection()
{
    int newPlayerID = nextFreeId();
    if(newPlayerID >= MAX_PLAYERS){
        QTextStream(stdout) << "Player wanted to connect but max players limit has been reached yet\n";
    }
    else{
        _playerSocket[newPlayerID] = nextPendingConnection();
        if(sendPlayerInfo(newPlayerID) > 0){
            QTextStream(stdout) << "[Player " << newPlayerID << "] CONNECTED\n";

            sendMap(newPlayerID);

            connect(_playerSocket[newPlayerID], &QWebSocket::disconnected, this, [this, newPlayerID](){this->playerDisconnected(newPlayerID);});
            connect(_playerSocket[newPlayerID], &QWebSocket::binaryMessageReceived, this, &GameServer::playerRequestHandle);
        }
        else{
            QTextStream(stdout) << "[Player " << newPlayerID << "] UNREACHABLE\n";
            _playerSocket[newPlayerID]->close();
        }
    }
}

// PLAYER DISCONNECTED
void GameServer::playerDisconnected(int playerID)
{
    QTextStream(stdout) << "[Player " << playerID << "] DISCONNECTED\n";
    resetPlayersStats();
    _movesList.clear();
    sendMovesList();
    _playerSocket[playerID] = nullptr;
}

// PLAYER SENT MESSAGE
void GameServer::playerRequestHandle(const QByteArray& arr)
{
    QJsonObject objectRequest = QJsonDocument().fromBinaryData(arr).object();

    QString type = objectRequest.value("type").toString();
    if(allConnected()){
        int playerID = objectRequest.value("playerID").toInt();
        if(type == "move"){
            QTextStream(stdout) << "[Player " << playerID << "] MOVED\n";
            _movesList.push_back(GameMove::fromJsonObject(objectRequest));
            sendMovesList();
        } else if(type == "ready"){
            QTextStream(stdout) << "[Player " << playerID << "] READY\n";
            _playersReady[playerID] = true;
            if(allReady()){
                letPlayersGo();
            }
        } else if(type == "unready"){
            QTextStream(stdout) << "[Player " << playerID << "] NOT READY\n";
            _playersReady[playerID] = false;
        } else if(type == "finished" || type == "unfinished"){
            if(type == "finished"){
                QTextStream(stdout) << "[Player " << playerID << "] FINISHED\n";
                _playersFinished[playerID] = true;
            }
            else{
                QTextStream(stdout) << "[Player " << playerID << "] NOT FINISHED\n";
                _playersFinished[playerID] = false;
            }

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

//--------------------------------------------------------------

// CHECKING STATUSES OF ALL PLAYERS

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
