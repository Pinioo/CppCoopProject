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
private:
    int _playerID;
    MoveType _moveType;
public:
    GameMove();
    GameMove(GameMove&&);
    GameMove(const GameMove&);
    GameMove(int, MoveType);

    GameMove& operator=(const GameMove&);

    static GameMove fromJsonObject(const QJsonObject&);
    QJsonObject toJsonObject() const;
    QString toString() const;
    int playerID() const {return _playerID;}
    int moveType() const {return _moveType;}
};

#endif // GAMEMOVE_H
