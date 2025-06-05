#include "cell.h"

Cell::Cell(int row, int col, QPushButton* button)
    : m_row(row), m_col(col), m_button(button), m_state(CellState::Empty) {
    updateAppearance();
}

int Cell::row() const { return m_row; }
int Cell::col() const { return m_col; }
QPushButton* Cell::button() const { return m_button; }

CellState Cell::state() const {
    return m_state;
}

void Cell::setState(CellState state) {
    m_state = state;
    m_preview = false;
    updateAppearance();
}

void Cell::setPlayerOwned(bool player) {
    m_isPlayerCell = player;
}

void Cell::setHidden(bool hidden) {
    m_hidden = hidden;
    updateAppearance();
}

void Cell::setPreview(bool preview) {
    if (m_state != CellState::Empty) return;
    m_preview = preview;
    updateAppearance();
}

void Cell::updateAppearance() {
    if (m_preview) {
        m_button->setStyleSheet("background-color: #a0d2eb; border: 1px solid #666;");
        return;
    }

    if (m_hidden && m_state == CellState::Ship) {
        m_button->setStyleSheet("background-color: lightblue; border: 1px solid #666;");
        return;
    }

    switch (m_state) {
    case CellState::Empty:
        m_button->setStyleSheet("background-color: lightblue; border: 1px solid #666;");
        break;
    case CellState::Ship:
        m_button->setStyleSheet("background-color: #5555ff; border: 1px solid #666;");
        break;
    case CellState::Hit:
        m_button->setStyleSheet("background-color: #ff5555; border: 1px solid #666;");
        m_button->setText("✖");
        break;
    case CellState::Miss:
        m_button->setStyleSheet("background-color: white; border: 1px solid #666;");
        m_button->setText("•");
        break;
    }
}
