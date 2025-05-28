#include "mainwindowSB.h"
#include "ui_mainwindowSB.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupPlayerField();
    setupEnemyField();

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartGame);
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_enemyAI;
    delete m_enemyShooter;
}

void MainWindow::setupPlayerField() {
    m_playerField = new GameField(this, ui->widget_3);
}

void MainWindow::setupEnemyField() {
    m_enemyField = new GameField(this, ui->_2);
}

void MainWindow::onStartGame() {
    ui->label->setText("Игра началась!");

    m_enemyAI = new EnemyAI(m_enemyField);
    m_enemyAI->placeShips();

    m_enemyShooter = new EnemyShooter(m_playerField);

    QTimer::singleShot(1000, this, &MainWindow::onEnemyTurn);
}

void MainWindow::onEnemyTurn() {
    if (!m_enemyShooter) return;

    QPoint shot = m_enemyShooter->makeShot();
    Cell* cell = m_playerField->cellAt(shot.y(), shot.x());

    bool hit = (cell->state() == CellState::Ship);
    cell->setState(hit ? CellState::Hit : CellState::Miss);

    m_enemyShooter->processShotResult(shot, hit);

    ui->label->setText(QString("Враг стреляет: %1, %2 — %3")
                           .arg(shot.x()).arg(shot.y()).arg(hit ? "попадание" : "мимо"));
}
