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

private:
    GameField* m_playerField;
    std::vector<QPoint> m_targets;
    std::vector<QPoint> m_visited;

    std::vector<QPoint> getAdjacentPoints(const QPoint& point) const;
    bool isValid(const QPoint& point) const;
    bool isVisited(const QPoint& point) const;
};

#endif // ENEMYSHOT_H
