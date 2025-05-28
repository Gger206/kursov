#pragma once
#ifndef MAINWINDOWSB_H
#define MAINWINDOWSB_H

#include <QMainWindow>
#include "gamefield.h"
#include "enemyfleet.h"
#include "enemyshot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartGame();
    void onEnemyTurn();

private:
    Ui::MainWindow *ui;

    GameField* m_playerField;
    GameField* m_enemyField;

    EnemyAI* m_enemyAI;
    EnemyShooter* m_enemyShooter;

    void setupPlayerField();
    void setupEnemyField();
};

#endif // MAINWINDOWSB_H
