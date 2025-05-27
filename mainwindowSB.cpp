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
}

void MainWindow::setupPlayerField() {
    m_playerField = new GameField(this, ui->widget_3);
}

void MainWindow::setupEnemyField() {
    m_enemyField = new GameField(this, ui->_2);
}

void MainWindow::onStartGame() {
    ui->label->setText("Игра началась!");
}
