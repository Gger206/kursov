#ifndef SCORE_H
#define SCORE_H

#include <QObject>
#include <QSettings>

class Score : public QObject
{
    Q_OBJECT
public:
    explicit Score(QObject *parent = nullptr);

    int wins() const;
    int losses() const;
    int totalGames() const;

    void addWin();
    void addLoss();

    void save() const;
    void load();

signals:
    void statsChanged();

private:
    int m_wins;
    int m_losses;
};

#endif // SCORE_H
