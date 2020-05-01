#include "gamemove.h"

GameMove::GameMove() : QObject(){

}

GameMove::GameMove(GameMove&& other) : QObject()
{
    int tmp = _playerID;
    _playerID = other._playerID;
    other._playerID = tmp;

    MoveType moveTmp = _moveType;
    _moveType = other._moveType;
    other._moveType = moveTmp;
}

GameMove::GameMove(const GameMove& other) : QObject()
{
    *this = other;
}

GameMove::GameMove(int playerID, MoveType moveType) : QObject(), _playerID(playerID), _moveType(moveType){

}

GameMove& GameMove::operator=(const GameMove& other)
{
    _playerID = other._playerID;
    _moveType = other._moveType;
    return *this;
}

GameMove GameMove::fromJsonObject(const QJsonObject& obj)
{
    int id = obj.value("playerID").toInt();
    QString moveTypeStr = obj.value("move").toString();
    MoveType moveType;
    if(moveTypeStr == "LEFT")
        moveType = LEFT;
    else if(moveTypeStr == "RIGHT")
        moveType = RIGHT;
    else
        moveType = GO;
    return GameMove(id, moveType);
}

QString GameMove::toString() const
{
    switch (_moveType) {
        case LEFT:
            return "Left";

        case RIGHT:
            return "Right";

        case GO:
            return "Go";

        default:
            return "";
    }
}

QJsonObject GameMove::toJsonObject() const
{
    QJsonObject obj;
    obj.insert("type", "move");
    obj.insert("playerID", _playerID);
    switch (_moveType) {
        case LEFT:
            obj.insert("move", "LEFT");
            break;
        case RIGHT:
            obj.insert("move", "RIGHT");
            break;
        case GO:
            obj.insert("move", "GO");
            break;
    }

    return obj;
}
