//实现了数独基本功能，和鼠标点击键盘输入逻辑，点击逻辑，判断输赢逻辑，计时器逻辑
//后续可添加功能：自定义地图，提供地图解法，和部分按钮进行图标美化
#include "sudokugame.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include<QTimer>
#include<QTime>
#include<QEvent>
#include<QMouseEvent>
#include<QGraphicsOpacityEffect>

SudokuGame::SudokuGame(QWidget *parent) : QWidget(parent)
{
    setFixedSize(1800, 1200);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框对话框

    // 设置窗口位置，使其向下移动300像素
    move(100, 300); // 这里假设初始位置为(100, 300)，可以根据需要调整

    // 设置背景颜色和透明度
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    // 添加阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadowEffect);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);
    // 创建地图函数
    createGrid();

    // 创建计时器标签并初始化为 00:00
    timerLabel = new QLabel("00:00", this);
    timerLabel->move(1300, 10);
    // 将计时器标签移到右上角
    QFont font("Arial", 30);
    timerLabel->setFont(font);

    // 启动计时器
    timer = new QTimer(this);
    elapsedTime = new QTime(0, 0, 0);
    connect(timer, &QTimer::timeout, this, &SudokuGame::updateTime);
    timer->start(1000); // 1000毫秒 = 1秒

    // 创建清空按钮、帮助按钮、新游戏按钮、重玩按钮和暂停按钮，设置字体为中文
    QPushButton *clearButton = new QPushButton("清空", this);
    QPushButton *helpButton = new QPushButton("帮助", this);
    QPushButton *newGameButton = new QPushButton("开始游戏", this);
    QPushButton *restartButton = new QPushButton("重新开始", this);
    QPushButton *pauseButton = new QPushButton("暂停", this);
    QPushButton *submitButton = new QPushButton("提交", this);
    QPushButton *returnButton=new QPushButton("返回",this);
    QString buttonStyle =   "QPushButton {"
                          "background-color: transparent; border: 2px solid white; border-radius: 10px; "
                          "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                          "}"
                          "QPushButton:hover {"
                          "color: lightgray;"  // 悬停时的颜色变化
                          "}";
    clearButton->setStyleSheet(buttonStyle);
    clearButton->setIcon(QIcon(":/new/images/qk.png"));
    clearButton->setIconSize(QSize(30, 30));
    helpButton->setStyleSheet(buttonStyle);
    helpButton->setIcon(QIcon(":/new/images/bz.png"));
    helpButton->setIconSize(QSize(30, 30));
    newGameButton->setStyleSheet(buttonStyle);
    newGameButton->setIcon(QIcon(":/new/images/ks.png"));
    newGameButton->setIconSize(QSize(30, 30));
    restartButton->setStyleSheet(buttonStyle);
    restartButton->setIcon(QIcon(":/new/images/cxks.png"));
    restartButton->setIconSize(QSize(30, 30));
    pauseButton->setStyleSheet(buttonStyle);
    pauseButton ->setIcon(QIcon(":/new/images/zt.png"));
    pauseButton ->setIconSize(QSize(30, 30));
    submitButton->setStyleSheet(buttonStyle);
    submitButton->setIcon(QIcon(":/new/images/tj.png"));
    submitButton->setIconSize(QSize(30, 30));
    returnButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");
    returnButton->setIcon(QIcon(":/new/images/return.png"));
    returnButton->setIconSize(QSize(30, 30));

    // 设置按钮大小和样式
    clearButton->setFixedSize(220, 60);
    helpButton->setFixedSize(220, 60);
    newGameButton->setFixedSize(220, 60);
    restartButton->setFixedSize(220, 60);
    pauseButton->setFixedSize(220, 60);
    submitButton->setFixedSize(220, 60);
    returnButton->setFixedSize(220,60);

    // 添加点击按钮的信号槽连接
    connect(clearButton, &QPushButton::clicked, this, &SudokuGame::clearGrid);
    connect(helpButton, &QPushButton::clicked, this, &SudokuGame::showHelp);
    connect(newGameButton, &QPushButton::clicked, this, &SudokuGame::startNewGame);
    connect(restartButton, &QPushButton::clicked, this, &SudokuGame::restartGame);
    connect(pauseButton, &QPushButton::clicked, this, &SudokuGame::pauseGame);
    connect(submitButton, &QPushButton::clicked, this, &SudokuGame::submitGrid);
    connect(returnButton,&QPushButton::clicked,this,&SudokuGame::returnToMainPage);

    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(helpButton);
    buttonLayout->addWidget(newGameButton);
    buttonLayout->addWidget(restartButton);
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(pauseButton);
    //buttonLayout->addWidget(returnButton);

    // 设置按钮之间的间距
    buttonLayout->setSpacing(0);

    // 向上偏移一点
    buttonLayout->setContentsMargins(0, 0, 200, 300);

    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    // 添加计时器标签
    mainLayout->addWidget(returnButton, 0, Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(gridWidget);
    // 使用居中的地图部件
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

//析构函数，删除地图
SudokuGame::~SudokuGame()
{
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            delete sudokuGrid[row][col];
        }
    }
}

//返回主页面
void SudokuGame::on_ReturnButton_clicked()
{
    emit returnToMainPage();
}

//创建数独地图
void SudokuGame::createGrid()
{
    // 创建包含81个小正方形的大正方形网格
    gridWidget = new QWidget(this);
    // 增大地图的大小
    gridWidget->setFixedSize(600, 600);

    QGridLayout *gridLayout = new QGridLayout(gridWidget);
    gridLayout->setSpacing(0);

    // 添加每个小正方形格子
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            QLabel *label = new QLabel(this);
            // 增大数字和格子的大小
            label->setFixedSize(60, 60);
            label->setFrameStyle(QFrame::Box);
            label->setLineWidth(2);
            label->setAlignment(Qt::AlignCenter);

            // 将初始地图中的数字填充到对应的格子中
            int num = initialMap[row][col];
            if (num != 0)
            //如果是有初始值的
            {
                label->setText(QString::number(num));
                // 设置数字的字体和大小
                QFont font("Arial", 24);
                label->setFont(font);
                // 设置初始数字格子为青色
                label->setStyleSheet("background-color: orange;");
            }
            //添加标签行列
            gridLayout->addWidget(label, row, col);
            sudokuGrid[row][col] = label;
            // 为每个标签安装事件过滤器，用来实现键盘输入1
            label->installEventFilter(this);
        }
    }
}

//填充数字格子
void SudokuGame::fillGrid(int num)
{
    // 确保游戏开始且处于运行状态
    if (!isPaused && !elapsedTime->isNull())
    {
        QWidget *currentCell = dynamic_cast<QWidget*>(focusWidget()->parent());
        if (currentCell)
        {
            int row = gridWidget->layout()->indexOf(currentCell) / 9;
            int col = gridWidget->layout()->indexOf(currentCell) % 9;
            if (sudokuGrid[row][col]->text().isEmpty())
            {
                sudokuGrid[row][col]->setText(QString::number(num));
            }
        }
    }
}


//清空函数，样式表和数字
void SudokuGame::clearGrid()
{
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            sudokuGrid[row][col]->clear();
            sudokuGrid[row][col]->setStyleSheet(""); // 清除样式表
        }
    }
}

//规则说明
void SudokuGame::showHelp()
{
    QMessageBox::information(this, "数独游戏规则", "数独游戏的目标是在一个9x9的网格中填入数字，使得每一行、每一列以及每一个3x3的子网格都包含从1到9的所有数字。初始网格可能包含一些数字，您不能更改它们。");
}

//开始游戏
void SudokuGame::startNewGame()
{
    // 启动计时器并归零
    timer->start(1000);
    elapsedTime->setHMS(0, 0, 0);
    isPaused = false;

    // 设置初始地图
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            int num = initialMap[row][col];
            if (num != 0)
            {
                QFont font("Arial", 24); // 设置数字的字体和大小
                sudokuGrid[row][col]->setText(QString::number(num));
                sudokuGrid[row][col]->setFont(font);
                sudokuGrid[row][col]->setStyleSheet("background-color: orange;"); // 设置初始数字格子为青色

            } else
            {
                sudokuGrid[row][col]->clear();
            }
        }
    }
}

//和开始游戏逻辑相似
void SudokuGame::restartGame()
{
    // 停止计时器
    timer->stop();
    // 将计时器归零
    *elapsedTime = QTime(0, 0, 0);
    timerLabel->setText("00:00");

    // 清空地图
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int num = initialMap[row][col];
            if (num != 0) {
                sudokuGrid[row][col]->setText(QString::number(num));
            } else {
                sudokuGrid[row][col]->clear();
            }
        }
    }

    // 重新启动计时器
    timer->start(1000);
    isPaused = false;
}


//暂停游戏逻辑
void SudokuGame::pauseGame()
{
    if (isPaused)
    {
        // 如果当前已经是暂停状态，恢复计时器
        timer->start(1000);
        isPaused = false;
        // 更新按钮文本为 "Pause"
        // 更新其他相关的暂停恢复操作
    } else
    {
        // 如果当前是运行状态，暂停计时器
        timer->stop();
        isPaused = true;
        // 更新按钮文本为 "Resume"
        // 更新其他相关的暂停操作
    }
}

//更新槽函数
void SudokuGame::updateTime()
{
    if (!isPaused)
    {
        *elapsedTime = elapsedTime->addSecs(1);
        QString timeStr = elapsedTime->toString("mm:ss");
        timerLabel->setText(timeStr);
    }
}



bool SudokuGame::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        // 检查是否是鼠标左键点击事件
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            // 检查是否点击的是 QLabel
            QLabel *clickedLabel = qobject_cast<QLabel*>(obj);
            if (clickedLabel)
            {
                int row = -1, col = -1;
                // 在sudokuGrid中查找点击的QLabel的位置
                for (int i = 0; i < 9; ++i)
                {
                    for (int j = 0; j < 9; ++j)
                    {
                        if (sudokuGrid[i][j] == clickedLabel)
                        {
                            row = i;
                            col = j;
                            break;
                        }
                    }
                    if (row != -1) break;
                }
                if (row != -1 && col != -1)
                {
                    // 如果找到了QLabel的位置
                    if (initialMap[row][col] == 0 || clickedLabel->text().isEmpty())
                    {
                        // 如果该位置没有初始值或者标签为空，则允许填入数字
                        clearShadowEffect();
                        // 添加阴影效果
                        addShadowEffect(row, col);
                        // 设置当前点击的格子获取焦点
                        clickedLabel->setFocus();

                    }
                }
            }
        }
    }
    else if (event->type() == QEvent::KeyPress)
    {
        // 检查是否是键盘按下事件
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        // 检查键盘按下的是否为数字键（从 Qt::Key_0 到 Qt::Key_9）
        if (keyEvent->key() >= Qt::Key_0 && keyEvent->key() <= Qt::Key_9)
        {
            // 如果点击的是 QLabel，则将按下的数字填入标签中
            QLabel *focusedLabel = qobject_cast<QLabel*>(this->focusWidget());
            if (focusedLabel)
            {
                // 获取当前标签的行和列
                int row = -1, col = -1;
                // 在sudokuGrid中查找点击的QLabel的位置
                for (int i = 0; i < 9; ++i)
                {
                    for (int j = 0; j < 9; ++j)
                    {
                        if (sudokuGrid[i][j] == focusedLabel)
                        {
                            row = i;
                            col = j;
                            break;
                        }
                    }
                    if (row != -1) break;
                }
                if (row != -1 && col != -1)
                {
                    // 如果找到了QLabel的位置
                    if (initialMap[row][col] == 0 || focusedLabel->text().isEmpty())
                    {
                        // 如果该位置没有初始值或者标签为空，则将按下的数字填入标签中
                        QFont font("Arial", 24);
                        // 设置数字的字体和大小
                        sudokuGrid[row][col]->setText(QString::number(keyEvent->key() - Qt::Key_0));
                        sudokuGrid[row][col]->setFont(font);
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}



bool SudokuGame::checkWin()
{
    // 检查每一行
    for (int row = 0; row < 9; ++row)
    {
        bool rowCheck[10] = {false};
        for (int col = 0; col < 9; ++col)
        {
            QString text = sudokuGrid[row][col]->text();
            if (text.isEmpty() || text.toInt() < 1 || text.toInt() > 9 || rowCheck[text.toInt()])
            {
                return false; // 如果该行有空格、数字不在1到9范围内或有重复数字，返回false
            }
            rowCheck[text.toInt()] = true;
        }
    }

    // 检查每一列
    for (int col = 0; col < 9; ++col)
    {
        bool colCheck[10] = {false};
        for (int row = 0; row < 9; ++row)
        {
            QString text = sudokuGrid[row][col]->text();
            if (text.isEmpty() || text.toInt() < 1 || text.toInt() > 9 || colCheck[text.toInt()])
            {
                return false; // 如果该列有空格、数字不在1到9范围内或有重复数字，返回false
            }
            colCheck[text.toInt()] = true;
        }
    }

    // 检查每一个3x3的子网格
    for (int startRow = 0; startRow < 9; startRow += 3)
    {
        for (int startCol = 0; startCol < 9; startCol += 3)
        {
            bool subGridCheck[10] = {false};
            for (int row = startRow; row < startRow + 3; ++row)
            {
                for (int col = startCol; col < startCol + 3; ++col)
                {
                    QString text = sudokuGrid[row][col]->text();
                    if (text.isEmpty() || text.toInt() < 1 || text.toInt() > 9 || subGridCheck[text.toInt()])
                    {
                        return false; // 如果该子网格有空格、数字不在1到9范围内或有重复数字，返回false
                    }
                    subGridCheck[text.toInt()] = true;
                }
            }
        }
    }

    return true; // 如果所有条件都满足，则返回true，表示游戏胜利
}


void SudokuGame::submitGrid()
{
    if (checkWin())
    {
        // 用户胜利
        QMessageBox::information(this, "恭喜", "您已经解决了数独！恭喜！");
    } else
    {
        // 用户失败
        QMessageBox::information(this, "失败", "很抱歉，您的解决方案不正确，请继续努力！");
    }
}


void SudokuGame::addShadowEffect(int row, int col)
{
    // 添加阴影效果到当前点击的格子
    sudokuGrid[row][col]->setStyleSheet("background-color: #808080;"); // 添加阴影效果
    // 添加阴影效果到同一行和同一列的格子
    for (int i = 0; i < 9; ++i)
    {
        sudokuGrid[row][i]->setStyleSheet("background-color: #C0C0C0;"); // 添加行阴影效果
        sudokuGrid[i][col]->setStyleSheet("background-color: #C0C0C0;"); // 添加列阴影效果
    }
    // 计算正方形格子的起始行和列
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    // 添加阴影效果到同一正方形内的格子
    for (int i = startRow; i < startRow + 3; ++i)
    {
        for (int j = startCol; j < startCol + 3; ++j)
        {
            sudokuGrid[i][j]->setStyleSheet("background-color: #C0C0C0;"); // 添加正方形阴影效果
        }
    }
    checkConflicts(row, col);
}


void SudokuGame::clearShadowEffect()
{
    // 遍历所有格子，清除阴影效果
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            sudokuGrid[row][col]->setStyleSheet(""); // 清除样式表
        }
    }
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int num = initialMap[row][col];
            if (num != 0) {
                sudokuGrid[row][col]->setStyleSheet("background-color: orange;"); // 设置初始数字格子为青色
            } else {

            }
        }
    }
}


void SudokuGame::checkConflicts(int row, int col)
{
    // 获取当前填入的数字
    QString currentText = sudokuGrid[row][col]->text();

    if(currentText=="")
    {
        return;
    }

    // 检查同一行是否有相同的数字，如果有，将其设置为红色
    for (int i = 0; i < 9; ++i)
    {
        if (i != col && sudokuGrid[row][i]->text() == currentText)
        {
            sudokuGrid[row][i]->setStyleSheet("background-color: #FF0000;"); // 设置为红色
        }
    }

    // 检查同一列是否有相同的数字，如果有，将其设置为红色
    for (int i = 0; i < 9; ++i)
    {
        if (i != row && sudokuGrid[i][col]->text() == currentText)
        {
            sudokuGrid[i][col]->setStyleSheet("background-color: #FF0000;"); // 设置为红色
        }
    }

    // 计算正方形格子的起始行和列
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    // 检查同一正方形内是否有相同的数字，如果有，将其设置为红色
    for (int i = startRow; i < startRow + 3; ++i)
    {
        for (int j = startCol; j < startCol + 3; ++j)
        {
            if (!(i == row && j == col) && sudokuGrid[i][j]->text() == currentText)
            {
                sudokuGrid[i][j]->setStyleSheet("background-color: #FF0000;"); // 设置为红色
            }
        }
    }

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            int num = initialMap[row][col];
            if (num != 0)
            {
                sudokuGrid[row][col]->setStyleSheet("background-color: orange;"); // 设置初始数字格子为青色
            } else
            {

            }
        }
    }
}
