#include "game.h"

#include "q_includes.h"

Game::Game() :
    QObject(nullptr),
    myMode(HOST),
    _map(new Map::Map(15,15))
{
    mapDraw = new MapDrawing(_map);
    _map->makeDefault();

    connect(&_mySocket, &QAbstractSocket::disconnected, this, &Game::disconnected);
    connect(&_mySocket, &QAbstractSocket::connected, this, &Game::connected);

    _players[0] = QSharedPointer<Map::MapMovableCharacter>(new Map::MapMovableCharacter(_map, 8, 8));
    _players[1] = QSharedPointer<Map::MapMovableCharacter>(new Map::MapMovableCharacter(_map, 4, 4));

    _map->placeMovable(_players[0]);
    _map->placeMovable(_players[1]);
}

void Game::requestStepForward(int id)
{
    if(id > 2)
        qDebug() << "Player with id " << id << " doesn't exist";
    else{
        _players[id]->stepForward();
    }
}

void Game::requestRightTurn(int id)
{

    if(id > 2)
        qDebug() << "Player with id " << id << " doesn't exist";
    else
        _players[id]->rotateRight();

    QJsonObject mainObject;
    mainObject.insert("id", id);
    mainObject.insert("move", "RIGHT");
    _mySocket.write(QJsonDocument(mainObject).toBinaryData());
}

void Game::requestLeftTurn(int id)
{

    if(id > 2)
        qDebug() << "Player with id " << id << " doesn't exist";
    else
        _players[id]->rotateLeft();
}

bool Game::connectToServer(QString ipAddress, int port)
{
    _mySocket.connectToHost(ipAddress, port);
    if(_mySocket.waitForConnected(5000)){
        try{
            if(!_mySocket.waitForReadyRead(5000))
                throw new std::exception();
            QByteArray received;
            received = _mySocket.read(128);

            QJsonDocument responseJson = QJsonDocument().fromBinaryData(received);
            qDebug() << responseJson.toJson();
            if(!responseJson.isObject())
                throw new std::exception();

            QJsonObject responseObject = responseJson.object();
            if(!responseObject.contains("givenID"))
                throw new std::exception();

            _playerID = responseObject.take("givenID").toInt();
            qDebug() << _playerID;
            return true;
        }catch(std::exception* e){
            _mySocket.disconnectFromHost();
            return false;
        }
    }
    else
        return false;
}

void Game::disconnected()
{
    emit this->sigDisconnect();
}

void Game::connected()
{
    emit this->sigConnect();
}
