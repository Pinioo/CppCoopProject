#include "gamewindow.h"
#include "ui_gamewindow.h"

#include <QStringListModel>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    game = new Game();

    disconnectedSocket();

    connect(game, &Game::sigDisconnect, this, &GameWindow::disconnectedSocket);
    connect(game, &Game::sigConnect, this, &GameWindow::connectedSocket);
    connect(game, &Game::newMap, this, &GameWindow::newMap);
    connect(game, &Game::mapChanged, this, &GameWindow::updatePaint);
    connect(game, &Game::movesListChanged, this, &GameWindow::movesListChanged);
    connect(game, &Game::sigReady, this, &GameWindow::readyChanged);
    connect(game, &Game::playersMoving, this, &GameWindow::lockReady);
}

GameWindow::~GameWindow() {
    delete ui;
}

void GameWindow::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);

    QPainter painter;
    painter.begin(this);

    painter.drawImage(0, 0, game->mapDraw->image);

    painter.end();
}

void GameWindow::updatePaint(){
    repaint();
}

void GameWindow::on_goButton_released()
{
    game->requestStepForward(0);
    game->mapDraw->drawMap();
    repaint();
}

void GameWindow::on_rightButton_released()
{
    game->requestRightTurn(0);
    game->mapDraw->drawMap();
    repaint();
}

void GameWindow::on_leftButton_released()
{
    game->requestLeftTurn(0);
    game->mapDraw->drawMap();
    repaint();
}

void GameWindow::on_readyButton_released()
{
    game->ready();
    repaint();
}

void GameWindow::on_connectButton_released()
{
    game->connectToServer(ui->urlInput->text());
}

void GameWindow::connectedSocket()
{
    ui->connectLabel->setText("Connected");
    unlockMoves();
    ui->readyButton->setEnabled(true);
    ui->connectButton->setEnabled(false);
    ui->urlInput->setEnabled(false);
}

void GameWindow::readyChanged(bool ready)
{
    if(ready){
        lockMoves();
        ui->readyButton->setText("Cancel ready");
    }
    else{
        unlockMoves();
        ui->readyButton->setText("Ready");
    }
}

void GameWindow::disconnectedSocket() {
    game->mapDraw->resetMap();
    repaint();
    ui->connectLabel->setText("Not connected");
    lockMoves();
    ui->readyButton->setEnabled(false);
    ui->connectButton->setEnabled(true);
    ui->urlInput->setEnabled(true);
}

void GameWindow::movesListChanged(QStringList list) {
    auto movesListModel = new QStringListModel(this);
    movesListModel->setStringList(list);
    ui->listView->setModel(movesListModel);
}

void GameWindow::lockReady() {
    ui->readyButton->setEnabled(false);
}

void GameWindow::unlockReady() {
    ui->readyButton->setEnabled(true);
}

void GameWindow::newMap(QColor playerColor) {
    ui->playerColorSlot->setScene(new QGraphicsScene());
    ui->playerColorSlot->setBackgroundBrush(QBrush(playerColor));
    ui->playerColorSlot->repaint();
    unlockMoves();
    unlockReady();
    updatePaint();
}

void GameWindow::lockMoves() {
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
    ui->goButton->setEnabled(false);
}

void GameWindow::unlockMoves() {
    ui->leftButton->setEnabled(true);
    ui->rightButton->setEnabled(true);
    ui->goButton->setEnabled(true);
}
