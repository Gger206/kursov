#include "ship.h"

Ship::Ship(int size) : size(size) {}

std::unique_ptr<Ship> Ship::create(int size) {
    if (size >= 1 && size <= 4) {
        return std::make_unique<Ship>(size);
    }
    return nullptr;
}

int Ship::getSize() const {
    return size;
}

QVector<QPoint> Ship::getPositions() const {
    return positions;
}

void Ship::setPosition(const QPoint& bow, bool horizontal) {
    this->horizontal = horizontal;
    positions.clear();
    for (int i = 0; i < size; ++i) {
        positions.append(horizontal ? QPoint(bow.x() + i, bow.y()) : QPoint(bow.x(), bow.y() + i));
    }
}

bool Ship::isHorizontal() const {
    return horizontal;
}

bool Ship::isSunk() const {
    return hits >= size;
}

void Ship::hit() {
    if (hits < size)
        ++hits;
}
