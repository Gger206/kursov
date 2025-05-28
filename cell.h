#pragma once
#ifndef CELL_H
#define CELL_H

#include <QPushButton>

enum class CellState {
    Empty,
    Ship,
    Hit,
    Miss
};

class Cell {
public:
    Cell(int row, int col, QPushButton* button);

    int row() const;
    int col() const;

    void setState(CellState state);
    CellState state() const;

    void updateAppearance();

private:
    int m_row;
    int m_col;
    CellState m_state;
    QPushButton* m_button;
};

#endif // CELL_H

