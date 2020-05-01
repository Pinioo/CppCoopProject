#include "gamemove.h"

#include <QJsonDocument>
#include <QTextStream>

GameMove::GameMove(int playerID, MoveType moveType) : QObject(), playerID(playerID), moveType(moveType)
{

}

GameMove* GameMove::fromJsonObject(const QJsonObject& obj)
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
    return new GameMove(id, moveType);
}

QJsonObject GameMove::toJsonObject()
{
    QJsonObject obj;
    obj.insert("type", "move");
    obj.insert("playerID", playerID);
    switch (moveType) {
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
