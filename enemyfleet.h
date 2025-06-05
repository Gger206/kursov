#ifndef ENEMYFLEET_H
#define ENEMYFLEET_H

#include "gamefield.h"
#include "ship.h"
#include <vector>
#include <memory>

class EnemyFleet {
public:
    EnemyFleet(GameField* field);
    void placeShips();

private:
    GameField* m_field;
    std::vector<std::unique_ptr<Ship>> m_ships;

    bool canPlaceShip(const QVector<QPoint>& positions) const;
    void markShipArea(const QVector<QPoint>& positions, bool markAsShip);
    bool tryPlaceShip(int size);
    void placeShipOfSize(int size, int count);
};

#endif // ENEMYFLEET_H
