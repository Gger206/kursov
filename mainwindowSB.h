#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "gamefield.h"
#include "enemyfleet.h"
#include "enemyshot.h"
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartGame();
    void enemyShoot();
    void onShipPlacementFinished();

private:
    Ui::MainWindow *ui;

    GameField* m_playerField = nullptr;
    GameField* m_enemyField = nullptr;

    EnemyFleet* m_enemyAI = nullptr;
    EnemyShooter* m_enemyShooter = nullptr;

    QTimer* m_enemyTimer;
    bool m_playerTurn = true;

    Player* m_player = nullptr;

    void setupPlayerField();
    void setupEnemyField();
    void playerShoot(int row, int col);
};

#endif // MAINWINDOW_H
