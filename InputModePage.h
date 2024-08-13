#ifndef INPUTMODEPAGE_H
#define INPUTMODEPAGE_H
#include "config.h"
#include <QWidget>
#include <QStack>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include<QVector>
#include<QTimer>
class CompetitionPage;
class InputModePage : public QWidget
{
    Q_OBJECT

public:
    explicit InputModePage(CompetitionPage* parent = nullptr);
    bool isButtonClicked;
    std::vector<int> hiddenMap;
    bool comparePuzzleWithHiddenMap();
    void undoLastOperation();
    //实现撤销操作
    void setSpecialSymbolForCell(QLabel *cell, int hiddenValue);
    QTimer *timer=new QTimer(this);
    void updateTimer();
    void update(const QString &filePath);
    void upset();
    QVBoxLayout *gridSizeLayout;
    void updatePuzzleGridSymbols();
    std::vector<std::pair<std::pair<int,int> ,int>> getans();

signals:
    void returnToCompetitionPage(); // 发出返回竞赛页面信号
    void gameFinished(bool victory);


protected:
    void mousePressEvent(QMouseEvent *event); // 新增鼠标按下事件处理
    std::array<int, 6> maxUseCounts; // 新增：存储每个按钮尺寸的最大使用次数
    std::array<int, 6> useCounts; // 新增：存储每个按钮尺寸的实际使用次数


private slots:
    void startGame();
    void on_ReturnButton_clicked();
    void on_PuzzleCell_clicked(QLabel *cell, int clickedRow, int clickedCol); // 新增槽函数
    void on_GridSizeButton_clicked(int size); // 新增槽函数
    void on_ResetButton_clicked();
    void on_SubmitButton_clicked();
    void openMapinputDialog();//打开输入的问题
    void showhint();


private:
    QPushButton *startButton;
    QPushButton *returnButton;
    QPushButton *gridSizeButtons[4]; // 新增四个按钮
    QLabel **puzzleCells;
    const int puzzleSize = 6;
    QGridLayout *puzzleGrid;
    int currentGridSize = 6; // 记录当前网格大小
    void showSuccessDialog();
    void showFailureDialog();
    QStack<QString> undoStack; // 新增：用于存储每一步操作的描述，便于回退
    QPushButton *undoButton;
    QLabel *timerLabel;
    QPushButton *resetButton;
    QPushButton *submitButton;
    QPushButton *editMapButton;
    bool isGameFinished = false;
    QMap<int, QString> symbolFilePaths = {
        {0, ":/new/images/px1.png"},
        {1, ":/new/images/px4.png"},
        {2, ":/new/images/px2.png"},
        {3, ":/new/images/px4.png"},
        {4, ":/new/images/px3.png"}
    };
    int elapsedTimeSeconds = 0; // 跟踪已经过去的时间
    static void readHiddenMapFromFile(const QString &filePath, std::vector<int> &hiddenMap);
};


#endif // INPUTMODEPAGE_H
