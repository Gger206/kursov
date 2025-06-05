#include "enemyfleet.h"
#include <QRandomGenerator>

EnemyFleet::EnemyFleet(GameField* field)
    : m_field(field) {}

void EnemyFleet::placeShips() {
    // Правильное количество кораблей:
    // 1x4, 2x3, 3x2, 4x1
    placeShipOfSize(4, 1); // 1 четырехпалубный
    placeShipOfSize(3, 2); // 2 трехпалубных
    placeShipOfSize(2, 3); // 3 двухпалубных
    placeShipOfSize(1, 4); // 4 однопалубных
}

void EnemyFleet::placeShipOfSize(int size, int count) {
    for (int i = 0; i < count; ++i) {
        bool placed = false;
        int attempts = 0;
        const int maxAttempts = 100;

        while (!placed && attempts < maxAttempts) {
            attempts++;
            bool horizontal = QRandomGenerator::global()->bounded(2);
            int x = QRandomGenerator::global()->bounded(10 - (horizontal ? size : 0));
            int y = QRandomGenerator::global()->bounded(10 - (horizontal ? 0 : size));

            auto ship = Ship::create(size);
            ship->setPosition(QPoint(x, y), horizontal);

            if (canPlaceShip(ship->getPositions())) {
                markShipOnField(ship.get());
                m_ships.push_back(std::move(ship));
                placed = true;
            }
        }
    }
}

bool EnemyFleet::canPlaceShip(const QVector<QPoint>& positions) {
    for (const QPoint& pos : positions) {
        if (pos.x() < 0 || pos.x() >= 10 || pos.y() < 0 || pos.y() >= 10)
            return false;

        Cell* cell = m_field->cellAt(pos.y(), pos.x());
        if (!cell || cell->state() != CellState::Empty)
            return false;
    }
    return true;
}

void EnemyFleet::markShipOnField(Ship* ship) {
    for (const QPoint& pos : ship->getPositions()) {
        m_field->cellAt(pos.y(), pos.x())->setState(CellState::Ship);
    }
}
