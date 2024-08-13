#ifndef PLAYERDATAWINDOW_H
#define PLAYERDATAWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>

class PlayerDataWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerDataWindow(QWidget *parent = nullptr);
    void setPlayerInfo(const QString &username, const QString &avatarPath);
    void readGameData(const QString &username);

private:
    QLabel *titleLabel;
    QLabel *avatarLabel;
    QLabel *userInfoLabel;
    QVBoxLayout *mainLayout;
    QVBoxLayout *gameDataLayout;
    QList<QLabel *> gameDataLabels;

    void clearGameData();

    // 用于处理窗口拖动的事件处理函数
    QPoint m_dragPosition;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // PLAYERDATAWINDOW_H
