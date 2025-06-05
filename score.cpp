#include "score.h"

Score::Score(QObject *parent)
    : QObject(parent), m_wins(0), m_losses(0)
{
    load();
}

int Score::wins() const { return m_wins; }
int Score::losses() const { return m_losses; }
int Score::totalGames() const { return m_wins + m_losses; }

void Score::addWin()
{
    m_wins++;
    save();
    emit statsChanged();
}

void Score::addLoss()
{
    m_losses++;
    save();
    emit statsChanged();
}

void Score::save() const
{
    QSettings settings;
    settings.setValue("stats/wins", m_wins);
    settings.setValue("stats/losses", m_losses);
}

void Score::load()
{
    QSettings settings;
    m_wins = settings.value("stats/wins", 0).toInt();
    m_losses = settings.value("stats/losses", 0).toInt();
}
