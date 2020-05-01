#ifndef GAMEMOVE_H
#define GAMEMOVE_H

#include <QJsonObject>
#include <QObject>

class GameMove : public QObject
{
    Q_OBJECT
public:
    enum MoveType{
        LEFT, RIGHT, GO
    };

    int playerID;
    MoveType moveType;

    GameMove(int, MoveType);

    static GameMove* fromJsonObject(const QJsonObject&);
    QJsonObject toJsonObject();
};

#endif // GAMEMOVE_H
