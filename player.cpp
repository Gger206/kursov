#include "player.h"
#include <algorithm>
#include <QDebug>

Player::Player(GameField* field, QObject* parent)
    : QObject(parent), m_field(field), m_horizontal(true), m_placingShips(false) {
    m_availableShips = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
}

void Player::startPlacingShips() {
    m_placingShips = true;
    if (!m_availableShips.empty()) {
        m_currentShipSize = m_availableShips.front();
    }
    emit shipSelectionChanged();
}

bool Player::canPlaceShipAt(int row, int col, int size, bool horizontal) const {
    if (!m_field) return false;
    return m_field->canPlaceShip(row, col, size, horizontal);
}

void Player::placeShip(int row, int col, int size, bool horizontal) {
    auto ship = Ship::create(size);
    if (!ship) return;

    ship->setPosition(QPoint(col, row), horizontal);

    for (const QPoint& p : ship->getPositions()) {
        Cell* cell = m_field->cellAt(p.y(), p.x());
        if (cell) {
            cell->setState(CellState::Ship);
            cell->setPlayerOwned(true);
        }
    }

    m_ships.push_back(std::move(ship));
}

void Player::selectShip(int size) {
    if (std::find(m_availableShips.begin(), m_availableShips.end(), size) != m_availableShips.end()) {
        m_currentShipSize = size;
        emit shipSelectionChanged();
    }
}

void Player::rotateCurrentShip() {
    m_horizontal = !m_horizontal;
    emit shipSelectionChanged();
}

bool Player::allShipsPlaced() const {
    return m_availableShips.empty();
}

int Player::currentShipSize() const {
    return m_currentShipSize;
}

bool Player::isHorizontal() const {
    return m_horizontal;
}

const std::vector<int>& Player::availableShips() const {
    return m_availableShips;
}

QString Player::getShipName(int size) const {
    switch(size) {
    case 4: return "Линкор";
    case 3: return "Крейсер";
    case 2: return "Эсминец";
    case 1: return "Катер";
    default: return "Неизвестный";
    }
}

void Player::handleCellClick(int row, int col) {
    if (!m_placingShips || m_availableShips.empty()) return;

    if (m_currentShipSize <= 0) return;

    if (canPlaceShipAt(row, col, m_currentShipSize, m_horizontal)) {
        placeShip(row, col, m_currentShipSize, m_horizontal);

        auto it = std::find(m_availableShips.begin(), m_availableShips.end(), m_currentShipSize);
        if (it != m_availableShips.end()) {
            m_availableShips.erase(it);
        }

        if (!m_availableShips.empty()) {
            m_currentShipSize = m_availableShips.front();
            emit shipSelectionChanged();
        } else {
            m_placingShips = false;
            emit shipPlacementFinished();
        }
    }
}
