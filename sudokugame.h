#ifndef SUDOKUGAME_H
#define SUDOKUGAME_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
class SudokuGame : public QWidget
{
    Q_OBJECT

public:
    SudokuGame(QWidget *parent = nullptr);
    ~SudokuGame();
    bool checkWin();
    void addShadowEffect(int row, int col);
    void clearShadowEffect();
    void checkConflicts(int row, int col);
    void createGrid();
    void fillGrid(int num);
    void clearGrid();
    void showHelp();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QWidget *gridWidget; // 修改这里
    QLabel *sudokuGrid[9][9];
    int initialMap[9][9] =
        {
            {5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        };
    QTimer *timer;
    QTime *elapsedTime;
    bool isPaused;
    QLabel *timerLabel;

private slots:
    void startNewGame();
    void restartGame();
    void pauseGame();
    void updateTime();
    void submitGrid();
    void on_ReturnButton_clicked();

signals:
    void returnToMainPage(); // 发送返回主页面信号
};

#endif // SUDOKUGAME_H
