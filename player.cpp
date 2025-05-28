#include "player.h"
#include <QDebug>

Player::Player(GameField* field, QObject* parent)
    : QObject(parent), m_field(field), m_horizontal(true), m_placingShips(false)
{
    m_shipsToPlace = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
}

void Player::startPlacingShips() {
    m_placingShips = true;
    m_currentShipSize = m_shipsToPlace.front();
}

bool Player::allShipsPlaced() const {
    return m_shipsToPlace.empty();
}

void Player::handleCellClick(int row, int col) {
    if (!m_placingShips || m_shipsToPlace.empty()) return;

    if (canPlaceShipAt(row, col, m_currentShipSize, m_horizontal)) {
        placeShip(row, col, m_currentShipSize, m_horizontal);
        m_shipsToPlace.erase(m_shipsToPlace.begin());

        if (!m_shipsToPlace.empty()) {
            m_currentShipSize = m_shipsToPlace.front();
        } else {
            m_placingShips = false;
            emit shipPlacementFinished();
        }
    } else {
        qDebug() << "Нельзя разместить корабль здесь.";
    }
}

bool Player::canPlaceShipAt(int row, int col, int size, bool horizontal) const {
    for (int i = 0; i < size; ++i) {
        int x = horizontal ? col + i : col;
        int y = horizontal ? row : row + i;

        if (x >= 10 || y >= 10 || m_field->cellAt(y, x)->state() != CellState::Empty)
            return false;
    }
    return true;
}

void Player::placeShip(int row, int col, int size, bool horizontal) {
    auto ship = Ship::create(size);
    ship->setPosition(QPoint(col, row), horizontal);

    for (const QPoint& p : ship->getPositions()) {
        m_field->cellAt(p.y(), p.x())->setState(CellState::Ship);
    }

    m_ships.push_back(std::move(ship));
}

void Player::shootAt(int row, int col) {
    emit playerShot(row, col);
}
