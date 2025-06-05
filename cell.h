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
    QPushButton* button() const;
    CellState state() const;

    void setState(CellState state);
    void setPlayerOwned(bool player);
    void setHidden(bool hidden);
    void setPreview(bool preview);
    void updateAppearance();

private:
    int m_row;
    int m_col;
    CellState m_state;
    QPushButton* m_button;
    bool m_isPlayerCell = false;
    bool m_hidden = false;
    bool m_preview = false;
};

#endif // CELL_H
