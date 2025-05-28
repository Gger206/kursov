#ifndef PLAYER_H
#define PLAYER_H

#include "gamefield.h"
#include "ship.h"
#include <memory>
#include <vector>
#include <QObject>

class Player : public QObject {
    Q_OBJECT

public:
    Player(GameField* field, QObject* parent = nullptr);

    void startPlacingShips();
    void handleCellClick(int row, int col);
    void finalizePlacement();

    void shootAt(int row, int col);
    bool allShipsPlaced() const;

signals:
    void shipPlacementFinished();
    void playerShot(int row, int col);

private:
    GameField* m_field;
    std::vector<std::unique_ptr<Ship>> m_ships;
    std::vector<int> m_shipsToPlace;
    int m_currentShipSize;
    bool m_horizontal;
    bool m_placingShips;

    bool canPlaceShipAt(int row, int col, int size, bool horizontal) const;
    void placeShip(int row, int col, int size, bool horizontal);
};

#endif // PLAYER_H
