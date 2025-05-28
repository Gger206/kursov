#pragma once
#ifndef MAINWINDOWSB_H
#define MAINWINDOWSB_H

#include <QMainWindow>
#include "gamefield.h"
#include "enemyfleet.h"
#include "enemyshot.h"
#include <QTimer>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameField* m_playerField;
    GameField* m_enemyField;

    EnemyAI* m_enemyAI;
    EnemyShooter* m_enemyShooter;

    bool m_playerTurn = true;
    QTimer* m_enemyTimer;

    void setupPlayerField();
    void setupEnemyField();
    void onStartGame();
    void playerShoot(int row, int col);
    void enemyShoot();
    void nextTurn();
};
