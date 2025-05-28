#pragma once
#ifndef SHIP_H
#define SHIP_H

#include <QVector>
#include <QPoint>
#include <memory>

class Ship {
public:
    Ship(int size);

    static std::unique_ptr<Ship> create(int size);

    virtual ~Ship() = default;

    int getSize() const;
    QVector<QPoint> getPositions() const;
    void setPosition(const QPoint& bow, bool horizontal);
    bool isHorizontal() const;
    bool isSunk() const;
    void hit();

protected:
    int size;
    int hits = 0;
    QVector<QPoint> positions;
    bool horizontal = true;
};

#endif // SHIP_H
