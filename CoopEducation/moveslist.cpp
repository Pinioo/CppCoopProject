#include "moveslist.h"

MovesList::MovesList() : QVector<GameMove>()
{}

void MovesList::setId(int id)
{
    _playerId = id;
}

QStringList MovesList::toStringList() const
{
    QStringList movesStrList;
    for(const GameMove& move: *this){
        if(move.playerID() == _playerId)
            movesStrList.append("Me: " + move.toString());
        else
            movesStrList.append("Friend: " + move.toString());
    }
    return movesStrList;
}
