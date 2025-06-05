#include "mainwindowSB.h"
#include "ui_mainwindowSB.h"

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_playerField(nullptr), m_enemyField(nullptr),
    m_enemyAI(nullptr), m_enemyShooter(nullptr),
    m_playerTurn(true), m_player(nullptr)
{
    ui->setupUi(this);

    ui->shipTypeCombo->addItem("4 (Линкор)", 4);
    ui->shipTypeCombo->addItem("3 (Крейсер)", 3);
    ui->shipTypeCombo->addItem("2 (Эсминец)", 2);
    ui->shipTypeCombo->addItem("1 (Катер)", 1);

    setupFields();

    m_player = new Player(m_playerField, this);
    m_enemyTimer = new QTimer(this);

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartGame);
    connect(ui->rotateButton, &QPushButton::clicked, this, &MainWindow::onRotateClicked);
    connect(ui->shipTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onShipTypeChanged);
    connect(m_player, &Player::shipPlacementFinished,
            this, &MainWindow::onShipPlacementFinished);
    connect(m_player, &Player::shipSelectionChanged,
            this, &MainWindow::updatePlacementPreview);
    connect(m_enemyTimer, &QTimer::timeout, this, &MainWindow::enemyShoot);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupFields() {
    m_playerField = new GameField(ui->playerField, ui->gridLayout);
    m_enemyField = new GameField(ui->enemyField, ui->gridLayout_2);

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            QPushButton* btn = m_playerField->cellAt(row, col)->button();
            btn->installEventFilter(this);
            connect(btn, &QPushButton::clicked, [this, row, col]() {
                if (m_player && !m_player->allShipsPlaced()) {
                    m_player->handleCellClick(row, col);
                }
            });
        }
    }

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            QPushButton* btn = m_enemyField->cellAt(row, col)->button();
            disconnect(btn, &QPushButton::clicked, nullptr, nullptr);
            connect(btn, &QPushButton::clicked, [this, row, col]() {
                if (m_playerTurn) {
                    this->playerShoot(row, col);
                }
            });
        }
    }
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::Enter) {
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                if (m_playerField->cellAt(row, col)->button() == watched) {
                    showPlacementPreview(row, col);
                    return true;
                }
            }
        }
    }
    else if (event->type() == QEvent::Leave) {
        clearPreview();
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onStartGame() {
    ui->startButton->setEnabled(false);
    m_player->startPlacingShips();
    updatePlacementPreview();
}

void MainWindow::onRotateClicked() {
    if (m_player && !m_player->allShipsPlaced()) {
        m_player->rotateCurrentShip();
        updatePlacementPreview();
    }
}

void MainWindow::onShipTypeChanged(int index) {
    if (m_player && index >= 0) {
        int size = ui->shipTypeCombo->itemData(index).toInt();
        m_player->selectShip(size);
        updatePlacementPreview();
    }
}

void MainWindow::updatePlacementPreview() {
    clearPreview();

    if (!m_player || m_player->allShipsPlaced()) {
        ui->statusLabel->setText("Все корабли размещены");
        return;
    }

    int size = m_player->currentShipSize();
    ui->statusLabel->setText(
        QString("Размещение: %1 (%2 клетки)")
            .arg(m_player->getShipName(size))
            .arg(size)
        );

    // Обновление информации об оставшихся кораблях
    QStringList remaining;
    for (int s : m_player->availableShips()) {
        remaining << QString::number(s);
    }
    ui->shipsRemainingLabel->setText("Осталось: " + remaining.join(","));
}

void MainWindow::showPlacementPreview(int row, int col) {
    if (!m_player || m_player->allShipsPlaced()) return;

    int size = m_player->currentShipSize();
    bool horizontal = m_player->isHorizontal();

    for (int i = 0; i < size; ++i) {
        int x = horizontal ? col + i : col;
        int y = horizontal ? row : row + i;

        if (x < 10 && y < 10) {
            if (m_player->canPlaceShipAt(row, col, size, horizontal)) {
                m_playerField->cellAt(y, x)->setPreview(true);
            }
        }
    }
}

void MainWindow::clearPreview() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            m_playerField->cellAt(row, col)->setPreview(false);
        }
    }
}

void MainWindow::onShipPlacementFinished() {
    QMessageBox::information(this, "Готово", "Все корабли размещены! Игра начинается.");

    // Скрываем корабли противника
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            m_enemyField->cellAt(row, col)->setHidden(true);
            m_enemyField->cellAt(row, col)->setPlayerOwned(false);
        }
    }

    m_enemyAI = new EnemyFleet(m_enemyField);
    m_enemyAI->placeShips();
    m_enemyShooter = new EnemyShooter(m_playerField);

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            QPushButton* btn = m_enemyField->cellAt(row, col)->button();
            disconnect(btn, &QPushButton::clicked, nullptr, nullptr); // Отключаем все предыдущие соединения
            connect(btn, &QPushButton::clicked, [this, row, col]() {
                this->playerShoot(row, col);
            });
        }
    }
}

void MainWindow::playerShoot(int row, int col) {
    if (!m_playerTurn || !m_enemyField) return;

    Cell* cell = m_enemyField->cellAt(row, col);
    if (!cell) return;

    if (cell->state() == CellState::Hit || cell->state() == CellState::Miss) {
        return;
    }

    bool isHit = (cell->state() == CellState::Ship);

    if (isHit) {
        cell->setState(CellState::Hit);
        checkIfShipSunk(row, col);
    } else {
        cell->setState(CellState::Miss);
    }

    if (isHit) {
        if (checkWinCondition()) {
            QMessageBox::information(this, "Победа!", "Вы победили!");
            return;
        }
        m_playerTurn = true;
    } else {
        m_playerTurn = false;
        QTimer::singleShot(1000, this, &MainWindow::enemyShoot);
    }
}

void MainWindow::enemyShoot() {
    if (m_playerTurn) return;

    QPoint target = m_enemyShooter->makeShot();
    Cell* cell = m_playerField->cellAt(target.y(), target.x());

    if (!cell) return;

    if (cell->state() == CellState::Hit || cell->state() == CellState::Miss) {
        QTimer::singleShot(10, this, &MainWindow::enemyShoot);
        return;
    }

    bool isHit = (cell->state() == CellState::Ship);

    if (isHit) {
        cell->setState(CellState::Hit);
        checkIfShipSunk(target.y(), target.x());
    } else {
        cell->setState(CellState::Miss);
    }

    m_enemyShooter->processShotResult(target, isHit);

    if (isHit) {
        if (checkWinCondition()) {
            QMessageBox::information(this, "Поражение", "Компьютер победил!");
            return;
        }
        QTimer::singleShot(1000, this, &MainWindow::enemyShoot);
    } else {
        m_playerTurn = true;
    }
}

void MainWindow::checkIfShipSunk(int row, int col) {
    Q_UNUSED(row);
    Q_UNUSED(col);
}

bool MainWindow::checkWinCondition() {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            Cell* cell = m_enemyField->cellAt(row, col);
            if (cell && cell->state() == CellState::Ship) {
                return false;
            }
        }
    }
    return true;
}
