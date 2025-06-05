#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "cell.h"
#include <vector>
#include <QGridLayout>
#include <QWidget>

class GameField {
public:
    GameField(QWidget* parent, QGridLayout* layout);
    ~GameField();

    Cell* cellAt(int row, int col) const;
    const std::vector<std::vector<Cell*>>& cells() const;
    bool canPlaceShip(int startRow, int startCol, int size, bool horizontal) const;

private:
    QWidget* m_parentWidget;
    QGridLayout* m_gameLayout;
    std::vector<std::vector<Cell*>> m_cells;
};

#endif // GAMEFIELD_H
