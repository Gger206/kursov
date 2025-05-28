#include "cell.h"

Cell::Cell(int row, int col, QPushButton* button)
    : m_row(row), m_col(col), m_button(button), m_state(CellState::Empty) {
    updateAppearance();
}

int Cell::row() const { return m_row; }
int Cell::col() const { return m_col; }

void Cell::setState(CellState state) {
    m_state = state;
    updateAppearance();
}

CellState Cell::state() const {
    return m_state;
}

void Cell::updateAppearance() {
    switch (m_state) {
    case CellState::Empty:
        m_button->setStyleSheet("background-color: lightblue;");
        break;
    case CellState::Ship:
        m_button->setStyleSheet("background-color: gray;");
        break;
    case CellState::Hit:
        m_button->setStyleSheet("background-color: red;");
        break;
    case CellState::Miss:
        m_button->setStyleSheet("background-color: white;");
        break;
    }
}
