#include "enemyfleet.h"
#include <QRandomGenerator>
#include <algorithm>

EnemyFleet::EnemyFleet(GameField* field) : m_field(field) {}

void EnemyFleet::placeShips() {
    // Правила размещения: 1x4, 2x3, 3x2, 4x1
    placeShipOfSize(4, 1);
    placeShipOfSize(3, 2);
    placeShipOfSize(2, 3);
    placeShipOfSize(1, 4);
}

void EnemyFleet::placeShipOfSize(int size, int count) {
    for (int i = 0; i < count; ) {
        if (tryPlaceShip(size)) {
            i++;
        }
    }
}

bool EnemyFleet::tryPlaceShip(int size) {
    const int maxAttempts = 100;
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        bool horizontal = QRandomGenerator::global()->bounded(2);
        int x = QRandomGenerator::global()->bounded(10 - (horizontal ? size : 0));
        int y = QRandomGenerator::global()->bounded(10 - (horizontal ? 0 : size));

        if (m_field->canPlaceShip(y, x, size, horizontal)) {
            auto ship = Ship::create(size);
            ship->setPosition(QPoint(x, y), horizontal);

            for (const QPoint& p : ship->getPositions()) {
                m_field->cellAt(p.y(), p.x())->setState(CellState::Ship);
            }

            m_ships.push_back(std::move(ship));
            return true;
        }
    }
    return false;
}

bool EnemyFleet::canPlaceShip(const QVector<QPoint>& positions) const {
    if (!m_field) return false;

    return m_field->canPlaceShip(positions.first().y(),
                                 positions.first().x(),
                                 positions.size(),
                                 positions.first().y() == positions.last().y());
}

void EnemyFleet::markShipArea(const QVector<QPoint>& positions, bool markAsShip) {
    for (const QPoint& pos : positions) {
        Cell* cell = m_field->cellAt(pos.y(), pos.x());
        if (cell) {
            cell->setState(markAsShip ? CellState::Ship : CellState::Empty);
        }
    }
}
