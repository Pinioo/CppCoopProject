#include "game.h"

#include "q_includes.h"
#include "instanceof.h"
#include <QThread>

void Game::proceedMoves() {
    for(const GameMove& move: _movesList){
        switch (move.moveType()) {
            case GameMove::LEFT:
                _players[move.playerID()]->rotateLeft(); break;
            case GameMove::RIGHT:
                _players[move.playerID()]->rotateRight(); break;
            case GameMove::GO:
                _players[move.playerID()]->stepForward(); break;
        }
        _map->update();
        mapDraw->drawMap();
        emit this->mapChanged();
        QThread::msleep(500);
    }
    sendFinishedStatus();
    clearMovesList();
}

void Game::sendFinishedStatus() {
    int x = _players[_playerID]->x();
    int y = _players[_playerID]->y();
    QJsonObject obj;
    obj.insert("playerID", _playerID);
    if(instanceof<Map::MapFinish>(_map->getElem(x,y).data()) && _map->getElem(x,y).dynamicCast<Map::MapFinish>()->playerID() == _playerID){
        obj.insert("type", "finished");
    }
    else {
        obj.insert("type", "unfinished");
    }
    _mySocket.sendBinaryMessage(QJsonDocument(obj).toBinaryData());
}

void Game::clearMovesList(){
    _movesList.clear();
    emit this->movesListChanged(_movesList.toStringList());
}

Game::Game() :
    QObject(nullptr),
    _map(nullptr)
{
    mapDraw = new MapDrawing(_map);
    _playerID = -1;
    connect(&_mySocket, &QWebSocket::disconnected, this, &Game::disconnected);
    connect(&_mySocket, &QWebSocket::connected, this, &Game::connected);
    connect(&_mySocket, &QWebSocket::binaryMessageReceived, this, &Game::newData);
}

void Game::requestStepForward(int id){
    if(id > 2)
        qDebug() << "Player with id " << id << " doesn't exist";
    else{
        QJsonObject mainObject;
        mainObject.insert("type", "move");
        mainObject.insert("playerID", _playerID);
        mainObject.insert("move", "GO");
        _mySocket.sendBinaryMessage(QJsonDocument(mainObject).toBinaryData());
    }
}

void Game::requestRightTurn(int id){
    if(id > 2)
        qDebug() << "Player with id " << id << " doesn't exist";
    else{
        QJsonObject mainObject;
        mainObject.insert("type", "move");
        mainObject.insert("playerID", _playerID);
        mainObject.insert("move", "RIGHT");
        _mySocket.sendBinaryMessage(QJsonDocument(mainObject).toBinaryData());
    }
}

void Game::requestLeftTurn(int id){

    if(id > 2)
        qDebug() << "Player with id " << id << " doesn't exist";
    else {
        QJsonObject mainObject;
        mainObject.insert("type", "move");
        mainObject.insert("playerID", _playerID);
        mainObject.insert("move", "LEFT");
        _mySocket.sendBinaryMessage(QJsonDocument(mainObject).toBinaryData());
    }
}

void Game::connectToServer(QString url){
    _mySocket.open(QUrl(url));
}

void Game::ready(){
    QJsonObject readyObj;
    _ready = !_ready;
    if(_ready)
        readyObj.insert("type", "ready");
    else
        readyObj.insert("type", "unready");
    readyObj.insert("playerID", _playerID);
    _mySocket.sendBinaryMessage(QJsonDocument(readyObj).toBinaryData());
    emit this->sigReady(_ready);
}

void Game::disconnected(){
    clearMovesList();
    _playerID = -1;
    emit this->sigDisconnect();
}

void Game::connected(){
    emit this->sigConnect();
}

void Game::newData(const QByteArray& data){
    QJsonObject dataJson = QJsonDocument().fromBinaryData(data).object();

    QString type = dataJson.value("type").toString();

    if(type == "playerInfo"){
        _playerID = dataJson.value("givenID").toInt();
        _movesList.setId(_playerID);
    }
    else if(type == "map"){
        _map = Map::fromJsonObject(dataJson);
        for(int y = 0; y < _map->height(); ++y)
            for(int x = 0; x < _map->width(); ++x){
                QSharedPointer<Map::MapMovableCharacter> occupyingEl = _map->getElem(x,y)->occupyingElement().dynamicCast<Map::MapMovableCharacter>();
                if(occupyingEl != nullptr){
                    _players[occupyingEl->playerID()] = occupyingEl;
                }
            }

        _ready = false;
        emit this->sigReady(_ready);

        mapDraw->setMap(_map);
        mapDraw->drawMap();
        emit this->newMap(_players[_playerID]->color());
    }
    else if(type == "movelist"){
        QJsonArray movesArr = dataJson.value("movelist").toArray();
        _movesList.clear();
        for(QJsonValue move: movesArr){
            GameMove gameMove = GameMove::fromJsonObject(move.toObject());
            _movesList.push_back(gameMove);
        }
        QStringList movesStrList = _movesList.toStringList();
        emit this->movesListChanged(movesStrList);
    }
    else if(type == "go"){
        emit this->playersMoving();
        proceedMoves();
    }
}
