#include "enemyfleet.h"
#include <QRandomGenerator>

EnemyFleet::EnemyFleet(GameField* field)
    : m_field(field) {}

void EnemyFleet::placeShips() {
    placeLargeShipsAtEdges();
    placeSingleDeckShipsInCenter();
}

void EnemyFleet::placeLargeShipsAtEdges() {
    const std::vector<int> sizes = {4, 3, 3, 2, 2, 2};
    for (int size : sizes) {
        bool placed = false;
        while (!placed) {
            bool horizontal = QRandomGenerator::global()->bounded(2);
            int x = horizontal ? QRandomGenerator::global()->bounded(7) : (QRandomGenerator::global()->bounded(2) * 9); // край
            int y = horizontal ? (QRandomGenerator::global()->bounded(2) * 9) : QRandomGenerator::global()->bounded(7);

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

void EnemyFleet::placeSingleDeckShipsInCenter() {
    for (int i = 0; i < 4; ++i) {
        bool placed = false;
        while (!placed) {
            int x = 3 + QRandomGenerator::global()->bounded(4);
            int y = 3 + QRandomGenerator::global()->bounded(4);
            auto ship = Ship::create(1);
            ship->setPosition(QPoint(x, y), true);

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

        if (m_field->cellAt(pos.y(), pos.x())->state() != CellState::Empty)
            return false;
    }
    return true;
}

void EnemyFleet::markShipOnField(Ship* ship) {
    for (const QPoint& pos : ship->getPositions()) {
        m_field->cellAt(pos.y(), pos.x())->setState(CellState::Ship);
    }
}
