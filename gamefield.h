#pragma once
#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "cell.h"
#include <vector>
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

class GameField {
public:
    GameField(QWidget* parent, QGridLayout* layout);

    Cell* cellAt(int row, int col);
    const std::vector<std::vector<Cell*>>& cells() const;

private:
    std::vector<std::vector<Cell*>> m_cells;
};

#endif // GAMEFIELD_H
