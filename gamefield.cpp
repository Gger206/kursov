#include "gamefield.h"
#include "cell.h"
#include <QPushButton>
#include <QDebug>

GameField::GameField(QWidget* parent, QGridLayout* layout)
    : m_parentWidget(parent), m_gameLayout(layout)
{
    m_cells.resize(10);
    for (int row = 0; row < 10; ++row) {
        m_cells[row].resize(10);
        for (int col = 0; col < 10; ++col) {
            QPushButton* btn = new QPushButton();
            btn->setFixedSize(26, 26);
            m_gameLayout->addWidget(btn, row, col);

            Cell* cell = new Cell(row, col, btn);
            m_cells[row][col] = cell;

            QObject::connect(btn, &QPushButton::clicked, [=]() {
                qDebug() << "Button clicked at" << row << col;
            });
        }
    }
}

bool GameField::canPlaceShip(int startRow, int startCol, int size, bool horizontal) const {

    if (startRow < 0 || startCol < 0) return false;

    int endRow = startRow + (horizontal ? 0 : size - 1);
    int endCol = startCol + (horizontal ? size - 1 : 0);

    if (endRow >= 10 || endCol >= 10) return false;

    for (int i = 0; i < size; ++i) {
        int row = startRow + (horizontal ? 0 : i);
        int col = startCol + (horizontal ? i : 0);

        for (int r = row - 1; r <= row + 1; ++r) {
            for (int c = col - 1; c <= col + 1; ++c) {
                if (r >= 0 && r < 10 && c >= 0 && c < 10) {
                    const Cell* cell = cellAt(r, c);
                    if (cell && cell->state() == CellState::Ship) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

Cell* GameField::cellAt(int row, int col) const {
    if (row >= 0 && row < 10 && col >= 0 && col < 10) {
        return m_cells[row][col];
    }
    return nullptr;
}

const std::vector<std::vector<Cell*>>& GameField::cells() const {
    return m_cells;
}
