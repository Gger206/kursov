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
    explicit Player(GameField* field, QObject* parent = nullptr);

    void startPlacingShips();
    void selectShip(int size);
    void rotateCurrentShip();
    void handleCellClick(int row, int col);

    bool canPlaceShipAt(int row, int col, int size, bool horizontal) const;

    bool allShipsPlaced() const;
    int currentShipSize() const;
    bool isHorizontal() const;
    const std::vector<int>& availableShips() const;
    QString getShipName(int size) const;

signals:
    void shipPlacementFinished();
    void shipSelectionChanged();

private:
    GameField* m_field;
    std::vector<std::unique_ptr<Ship>> m_ships;
    std::vector<int> m_availableShips;
    int m_currentShipSize;
    bool m_horizontal;
    bool m_placingShips;

    void placeShip(int row, int col, int size, bool horizontal);
};

#endif // PLAYER_H
