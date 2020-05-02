#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "q_includes.h"
#include "gamemove.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

#include "mapmovablecharacter.h"
#include "mapdrawing.h"
#include "game.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    void paintEvent(QPaintEvent *event);
    void updatePaint();
private slots:
    void on_goButton_released();
    void on_rightButton_released();
    void on_leftButton_released();
    void on_readyButton_released();
    void on_connectButton_released();

    void disconnectedSocket();
    void connectedSocket();
    void readyChanged(bool);
    void movesListChanged(QStringList);
    void lockReady();
    void unlockReady();
    void newMap(QColor);

private:
    Game* game;
    Ui::GameWindow* ui;
    void lockMoves();
    void unlockMoves();
};
#endif // GAMEWINDOW_H
