#ifndef MAINWINDOWSB_H
#define MAINWINDOWSB_H

#include <QMainWindow>
#include <QTimer>
#include "gamefield.h"
#include "enemyfleet.h"
#include "enemyshot.h"
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onStartGame();
    void onRotateClicked();
    void onShipTypeChanged(int index);
    void enemyShoot();
    void onShipPlacementFinished();
    void updatePlacementPreview();

private:
    Ui::MainWindow *ui;
    GameField* m_playerField;
    GameField* m_enemyField;
    EnemyFleet* m_enemyAI;
    EnemyShooter* m_enemyShooter;
    QTimer* m_enemyTimer;
    bool m_playerTurn;
    Player* m_player;

    void setupFields();
    void playerShoot(int row, int col);
    void showPlacementPreview(int row, int col);
    void clearPreview();
};

#endif // MAINWINDOWSB_H
