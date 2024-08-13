#ifndef FREEMODEPAGE_H
#define FREEMODEPAGE_H

#include "config.h"
#include <QWidget>
#include <QStack>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QVector>
#include <QTimer>

class CompetitionPage;

// 定义拼图类型
enum PuzzleType
{
    PT_1x1,
    PT_2x2,
    PT_3x3,
    PT_4x4
};

// 定义拼图类
struct Puzzle
{
    PuzzleType type;
    int value; // 用于记录拼图的值（即被拼区域每次加一后的值）
    int x, y; // 拼图在地图中的位置

    Puzzle(PuzzleType type, int value, int x, int y) : type(type), value(value), x(x), y(y) {}
};

class FreeModePage : public QWidget
{
    Q_OBJECT

public:
    explicit FreeModePage(CompetitionPage* parent = nullptr);

    bool isButtonClicked;
    //按钮是否被点击，就是左侧的按钮
    std::vector<int> hiddenMap;
    //隐藏地图也就是那副正确答案
    bool comparePuzzleWithHiddenMap();
    //比较地图是不是赢了
    void undoLastOperation();
    // 实现撤销操作
    void setSpecialSymbolForCell(QLabel *cell, int hiddenValue);
    //设置地图图片
    QTimer *timer = new QTimer(this);
    //计时器
    void updateTimer();
    //更新计时器
    void update(const QString &filePath);
    //根据文件路径更新地图
    void upset();
    //重置
    QVBoxLayout *gridSizeLayout;  //右侧的按钮布局
    QString imagePath;
    // 头像路径
    QString _password;
    // 密码
    QString _username;
    void showhint();

signals:
    void returnToCompetitionPage(); // 发出返回竞赛页面信号
    void gameFinished(bool victory);//游戏结束信号，以供联机使用

protected:
    void mousePressEvent(QMouseEvent *event); // 新增鼠标按下事件处理
    std::array<int, 6> maxUseCounts; // 存储每个按钮尺寸的最大使用次数
    std::array<int, 6> useCounts; // 存储每个按钮尺寸的实际使用次数

private slots:
    void startGame();//开始游戏
    void on_ReturnButton_clicked();//返回按钮点击
    void on_PuzzleCell_clicked(QLabel *cell, int clickedRow, int clickedCol); //拼图点击
    void on_GridSizeButton_clicked(int size); // 盘面格子点击
    void on_ResetButton_clicked();//重置按钮点击
    void on_SubmitButton_clicked();//提交按钮点击
    std::vector<std::pair<std::pair<int,int> ,int>> getans();//得到答案
    void initMap();
private:
    QPushButton *startButton;
    QPushButton *returnButton;
    QPushButton *gridSizeButtons[4]; // 新增四个按钮
    QLabel **puzzleCells;//36个QLabel
    const int puzzleSize = 6;//固定盘面大小
    QGridLayout *puzzleGrid;//网格布局用来防止地图
    int currentGridSize = 6; // 记录当前网格大小
    void showSuccessDialog();//展示成功页面
    void showFailureDialog();//展示失败页面
    QStack<QString> undoStack; // 用于存储每一步操作的描述，便于回退
    QPushButton *undoButton; //撤回按钮
    QLabel *timerLabel;
    QPushButton *resetButton;//重置按钮
    QPushButton *submitButton;//提交按钮
    bool isGameFinished = false;//游戏是否结束按钮
    QMap<int, QString> symbolFilePaths = {
        {0, ":/new/images/px1.png"},
        {1, ":/new/images/px4.png"},
        {2, ":/new/images/px2.png"},
        {3, ":/new/images/px4.png"},
        {4, ":/new/images/px3.png"}
    };
    //图片路径问题
    int elapsedTimeSeconds = 0; // 跟踪已经过去的时间
    static void readHiddenMapFromFile(const QString &filePath, std::vector<int> &hiddenMap);
    //读取文件进去问题
};

#endif // FREEMODEPAGE_H
