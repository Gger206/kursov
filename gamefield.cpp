#include "gamefield.h"

GameField::GameField(QWidget* parent, QGridLayout* layout) {
    m_cells.resize(10);
    for (int row = 0; row < 10; ++row) {
        m_cells[row].resize(10);
        for (int col = 0; col < 10; ++col) {
            QPushButton* button = new QPushButton(parent);
            button->setFixedSize(30, 30);
            layout->addWidget(button, row, col);
            m_cells[row][col] = new Cell(row, col, button);
        }
    }
}

Cell* GameField::cellAt(int row, int col) {
    return m_cells[row][col];
}

const std::vector<std::vector<Cell*>>& GameField::cells() const {
    return m_cells;
}
