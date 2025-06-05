#include "enemyshot.h"
#include <cstdlib>
#include <ctime>

EnemyShooter::EnemyShooter(GameField* playerField)
    : m_playerField(playerField) {
    std::srand(std::time(nullptr));
}

QPoint EnemyShooter::makeShot() {
    QPoint target;

    while (!m_targets.empty()) {
        target = m_targets.back();
        m_targets.pop_back();
        if (!isVisited(target)) {
            m_visited.push_back(target);
            return target;
        }
    }

    do {
        target = QPoint(rand() % 10, rand() % 10);
    } while (isVisited(target));

    m_visited.push_back(target);
    return target;
}

void EnemyShooter::processShotResult(const QPoint& point, bool hit) {
    if (hit) {

        auto adj = getAdjacentPoints(point);
        for (const auto& p : adj) {
            if (!isVisited(p)) {
                m_targets.push_back(p);
            }
        }
    }

    m_targets.erase(std::remove(m_targets.begin(), m_targets.end(), point), m_targets.end());
}

std::vector<QPoint> EnemyShooter::getAdjacentPoints(const QPoint& point) const {
    std::vector<QPoint> adj;

    const int dx[] = { -1, 1, 0, 0 };
    const int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        QPoint p(point.x() + dx[i], point.y() + dy[i]);
        if (isValid(p)) {
            adj.push_back(p);
        }
    }
    return adj;
}

bool EnemyShooter::isValid(const QPoint& point) const {
    return point.x() >= 0 && point.x() < 10 && point.y() >= 0 && point.y() < 10;
}

bool EnemyShooter::isVisited(const QPoint& point) const {
    return std::find(m_visited.begin(), m_visited.end(), point) != m_visited.end();
}
