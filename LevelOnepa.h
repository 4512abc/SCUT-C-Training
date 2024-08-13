#ifndef LEVELONEPA_H
#define LEVELONEPA_H
#include <QStack>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include<QVector>
#include<QTimer>

class LevelOnepa : public QWidget
{
    Q_OBJECT

public:
    explicit LevelOnepa(QWidget *parent = nullptr);
    ~LevelOnepa();
    QWidget *gridLayout;
    QHBoxLayout *mainLayout;
    bool isButtonClicked;
    std::vector<int> hiddenMap;
    bool comparePuzzleWithHiddenMap();
    void undoLastOperation();
    void setShadow();
    //实现撤销操作
    void setSpecialSymbolForCell(QLabel *cell, int hiddenValue);
    QTimer *timer=new QTimer(this);
    void updateTimer();
    void update(const QString &filePath);
    void upset();
    QVBoxLayout *gridSizeLayout;
    QString imagePath;
    //头像路径
    QString _password;
    //密码
    QString _username;
    QString _nowstate;
    std::vector<std::pair<std::pair<int,int> ,int>> getans();
    int victory=0;
    void showSuccessDialog();
    void showFailureDialog();
    //这里是为了联机对战在levelone而设置的一个值
    //现在的想法就是0的时候我还在玩这个游戏，此时胜负未分
    //然后当他是1的时候，就代表赢下了这个游戏，这个时候赢下的一方跳出你胜利了的图样
    //然后当他是2的时候，就代表他输了这个游戏，这个时候另外一方直接获胜，因为对方输了，
    void setVictory(int value);

signals:
    void returnToPracticePage(); // 发送返回练习页面信号
    void gameFinished(bool victory);
    void gameEnded();  // 胜负状态信号


protected:
    void mousePressEvent(QMouseEvent *event); // 新增鼠标按下事件处理
    std::array<int, 6> maxUseCounts; // 新增：存储每个按钮尺寸的最大使用次数
    std::array<int, 6> useCounts; // 新增：存储每个按钮尺寸的实际使用次数

public slots:
void on_ResetButton_clicked();

private slots:
    void startGame();
    void on_ReturnButton_clicked();
    void on_PuzzleCell_clicked(QLabel *cell, int clickedRow, int clickedCol); // 新增槽函数
    void on_GridSizeButton_clicked(int size); // 新增槽函数
    void on_SubmitButton_clicked();
    void showhint();

private:
    QPushButton *startButton;
    QPushButton *returnButton;
    QPushButton *gridSizeButtons[4]; // 新增四个按钮
    QLabel **puzzleCells;
    const int puzzleSize = 6;
    QGridLayout *puzzleGrid;
    int currentGridSize = 6; // 记录当前网格大小
    QStack<QString> undoStack; // 新增：用于存储每一步操作的描述，便于回退
    QPushButton *undoButton;
    QLabel *timerLabel;
    QPushButton *resetButton;
    QPushButton *submitButton;
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


#endif // LEVELONEPA_H
