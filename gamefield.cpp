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

Cell* GameField::cellAt(int row, int col) {
    if (row >= 0 && row < 10 && col >= 0 && col < 10) {
        return m_cells[row][col];
    }
    return nullptr;
}

const std::vector<std::vector<Cell*>>& GameField::cells() const {
    return m_cells;
}
