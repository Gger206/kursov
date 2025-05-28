#pragma once
#ifndef MAINWINDOWSB_H
#define MAINWINDOWSB_H

#include <QMainWindow>
#include "gamefield.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    GameField* m_playerField;
    GameField* m_enemyField;

    void setupPlayerField();
    void setupEnemyField();
    void onStartGame();
};

#endif // MAINWINDOWSB_H
