#include "mainwindowSB.h"
#include "ui_mainwindowSB.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_enemyAI(nullptr), m_enemyShooter(nullptr), m_enemyTimer(new QTimer(this))
{
    ui->setupUi(this);

    setupPlayerField();
    setupEnemyField();

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartGame);

    connect(m_enemyTimer, &QTimer::timeout, this, &MainWindow::enemyShoot);

    m_player = new Player(m_playerField, this);
    connect(m_player, &Player::shipPlacementFinished, this, &MainWindow::onShipPlacementFinished);
    m_player->startPlacingShips();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupPlayerField() {
    m_playerField = new GameField(this, ui->widget_3);
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            Cell* cell = m_playerField->cellAt(row, col);
            QPushButton* btn = cell->button();
            connect(btn, &QPushButton::clicked, [=]() {
                if (m_player) {
                    m_player->handleCellClick(row, col);
                }
            });
        }
    }
}

void MainWindow::setupEnemyField() {
    m_enemyField = new GameField(this, ui->_2);
}

void MainWindow::onStartGame() {
    ui->label->setText("Игра началась!");

    m_enemyAI = new EnemyFleet(m_enemyField);
    m_enemyAI->placeShips();

    m_enemyShooter = new EnemyShooter(m_playerField);

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            Cell* cell = m_enemyField->cellAt(row, col);
            QPushButton* btn = cell->button();
            connect(btn, &QPushButton::clicked, [=]() {
                if (m_playerTurn && cell->state() == CellState::Empty) {
                    playerShoot(row, col);
                }
            });
        }
    }
}

void MainWindow::playerShoot(int row, int col) {
    Cell* cell = m_enemyField->cellAt(row, col);
    if (cell->state() == CellState::Ship) {
        cell->setState(CellState::Hit);
        ui->label->setText("Попадание!");
    } else {
        cell->setState(CellState::Miss);
        ui->label->setText("Промах!");
        m_playerTurn = false;
        m_enemyTimer->start(1000);
    }
}

void MainWindow::enemyShoot() {
    if (m_playerTurn) {
        m_enemyTimer->stop();
        return;
    }

    QPoint target = m_enemyShooter->chooseTarget();
    if (!m_playerField->cellAt(target.y(), target.x())) return;

    Cell* cell = m_playerField->cellAt(target.y(), target.x());
    if (cell->state() == CellState::Ship) {
        cell->setState(CellState::Hit);
        ui->label->setText("ИИ попал!");
        m_enemyShooter->notifyHit(target);
        m_enemyTimer->start(800);
    } else {
        cell->setState(CellState::Miss);
        ui->label->setText("ИИ промахнулся!");
        m_enemyShooter->notifyMiss(target);
        m_enemyTimer->stop();
        m_playerTurn = true;
    }
}

void MainWindow::onShipPlacementFinished() {
    ui->label->setText("Все корабли расставлены. Начинаем бой!");
}
