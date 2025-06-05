#ifndef ENEMYSHOT_H
#define ENEMYSHOT_H

#include <QPoint>
#include <vector>
#include <optional>
#include "gamefield.h"

class EnemyShooter {
public:
    explicit EnemyShooter(GameField* playerField);
    QPoint makeShot();
    void processShotResult(const QPoint& point, bool hit);
    void resetTargetMode();

private:
    GameField* m_playerField;
    std::vector<QPoint> m_targets;
    std::vector<QPoint> m_visited;
    QPoint m_lastHit;
    bool m_targetMode = false;
    bool m_knownDirection = false;
    QPoint m_firstHit;
    int m_direction = 0;

    std::vector<QPoint> getAdjacentPoints(const QPoint& point, bool includeDiagonal = false) const;
    bool isValid(const QPoint& point) const;
    bool isVisited(const QPoint& point) const;
    QPoint findNextInDirection();
};

#endif // ENEMYSHOT_H
