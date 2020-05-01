#ifndef MOVESLIST_H
#define MOVESLIST_H

#include <QVector>
#include <QString>
#include <gamemove.h>

class MovesList : public QVector<GameMove>
{
private:
    int _playerId;

public:
    MovesList();
    void setId(int);
    QStringList toStringList() const;
};

#endif // MOVESLIST_H
