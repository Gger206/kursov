#include "enemyShot.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>

EnemyShooter::EnemyShooter(GameField* playerField)
    : m_playerField(playerField) {
    std::srand(std::time(nullptr));
}

QPoint EnemyShooter::makeShot() {

    if (m_targetMode && m_knownDirection) {
        QPoint next = findNextInDirection();
        if (isValid(next) && !isVisited(next)) {
            m_visited.push_back(next);
            return next;
        }
    }

    while (!m_targets.empty()) {
        QPoint target = m_targets.back();
        m_targets.pop_back();
        if (!isVisited(target)) {
            m_visited.push_back(target);
            return target;
        }
    }

    QPoint target;
    do {
        target = QPoint(rand() % 10, rand() % 10);
    } while (isVisited(target));

    m_visited.push_back(target);
    return target;
}

void EnemyShooter::processShotResult(const QPoint& point, bool hit) {
    if (hit) {
        m_lastHit = point;

        if (!m_targetMode) {
            m_targetMode = true;
            m_firstHit = point;
            auto adj = getAdjacentPoints(point);
            m_targets.insert(m_targets.end(), adj.begin(), adj.end());
        } else {
            if (!m_knownDirection) {
                if (m_firstHit.x() == point.x()) {
                    m_direction = 2;
                    m_knownDirection = true;
                } else if (m_firstHit.y() == point.y()) {
                    m_direction = 1;
                    m_knownDirection = true;
                }
            }

            if (m_knownDirection) {
                auto dirPoints = getAdjacentPoints(point, false);
                for (const auto& p : dirPoints) {
                    if ((m_direction == 1 && p.y() == point.y()) ||
                        (m_direction == 2 && p.x() == point.x())) {
                        if (!isVisited(p)) {
                            m_targets.push_back(p);
                        }
                    }
                }
            } else {
                auto adj = getAdjacentPoints(point);
                for (const auto& p : adj) {
                    if (!isVisited(p)) {
                        m_targets.push_back(p);
                    }
                }
            }
        }
    } else if (m_targetMode && !m_targets.empty()) {
        // Если промах в режиме преследования, пробуем другую сторону
        if (m_knownDirection) {
            m_targets.clear();
            auto dirPoints = getAdjacentPoints(m_firstHit, false);
            for (const auto& p : dirPoints) {
                if ((m_direction == 1 && p.y() == m_firstHit.y()) ||
                    (m_direction == 2 && p.x() == m_firstHit.x())) {
                    if (!isVisited(p)) {
                        m_targets.push_back(p);
                    }
                }
            }
        }
    }
}

bool EnemyShooter::isVisited(const QPoint& point) const {
    return std::find(m_visited.begin(), m_visited.end(), point) != m_visited.end();
}

QPoint EnemyShooter::findNextInDirection() {
    if (m_direction == 1) { // Горизонтально
        QPoint right(m_lastHit.x() + 1, m_lastHit.y());
        QPoint left(m_lastHit.x() - 1, m_lastHit.y());

        if (isValid(right) && !isVisited(right)) return right;
        if (isValid(left) && !isVisited(left)) return left;
    }
    else if (m_direction == 2) { // Вертикально
        QPoint down(m_lastHit.x(), m_lastHit.y() + 1);
        QPoint up(m_lastHit.x(), m_lastHit.y() - 1);

        if (isValid(down) && !isVisited(down)) return down;
        if (isValid(up) && !isVisited(up)) return up;
    }

    return QPoint(-1, -1); // Недействительная точка
}

std::vector<QPoint> EnemyShooter::getAdjacentPoints(const QPoint& point, bool includeDiagonal) const {
    std::vector<QPoint> adj;

    const int dx[] = { -1, 1, 0, 0 };
    const int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        QPoint p(point.x() + dx[i], point.y() + dy[i]);
        if (isValid(p)) {
            adj.push_back(p);
        }
    }

    if (includeDiagonal) {
        const int diagDx[] = { -1, -1, 1, 1 };
        const int diagDy[] = { -1, 1, -1, 1 };

        for (int i = 0; i < 4; ++i) {
            QPoint p(point.x() + diagDx[i], point.y() + diagDy[i]);
            if (isValid(p)) {
                adj.push_back(p);
            }
        }
    }

    return adj;
}

bool EnemyShooter::isValid(const QPoint& point) const {
    return point.x() >= 0 && point.x() < 10 && point.y() >= 0 && point.y() < 10;
}

void EnemyShooter::resetTargetMode() {
    m_targetMode = false;
    m_knownDirection = false;
    m_direction = 0;
    m_targets.clear();
}
