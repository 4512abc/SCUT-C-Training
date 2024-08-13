#include "LevelOnepa.h"
#include"SuccessDialog.h"
#include"FailureDialog.h"
#include <QRandomGenerator>
#include<QPalette>
#include<QMessageBox>
#include<QFile>
#include<QInputDialog>
#include<QGraphicsDropShadowEffect>
#include "CustomHintDialog.h"
#include "custommessagebox.h"
#include <QScreen>
LevelOnepa::LevelOnepa(QWidget *parent)
    : QWidget(parent)
{
    timerLabel = new QLabel("时间：0 秒", this);
    timerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white; border: 2px solid black; padding: 5px;");
    // 初始化隐藏地图数据
    hiddenMap.resize(36);
    readHiddenMapFromFile(QStringLiteral(":/new/images/qqzl.txt"), hiddenMap);
    QString buttonStyle =(   "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");

    maxUseCounts[0] = 1; // 1x1按钮最大使用次数为1次
    maxUseCounts[1] = 2; // 1x1按钮最大使用次数为1次
    maxUseCounts[2] = 2; // 2x2按钮最大使用次数为2次
    maxUseCounts[3] = 2; // 3x3按钮最大使用次数为2次
    maxUseCounts[4] = 1; // 4x4按钮最大使用次数为1次
    maxUseCounts[5] = 1; // 4x4按钮最大使用次数为1次
    maxUseCounts[6] = 1; // 4x4按钮最大使用次数为1次
    useCounts.fill(0); // 初始化所有计数器为0
    // 创建返回按钮
    isButtonClicked = false; // 新增：初始化标志变量为 false
    returnButton = new QPushButton("返回", this);
    returnButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");
    returnButton->setIcon(QIcon(":/new/images/return.png"));
    returnButton->setIconSize(QSize(40,40));

    connect(returnButton, &QPushButton::clicked, this, &LevelOnepa::returnToPracticePage);
    resetButton = new QPushButton("重置", this);
    resetButton->setStyleSheet(buttonStyle);
    resetButton->setIcon(QIcon(":/new/images/cxks.png"));
    resetButton->setIconSize(QSize(41, 41));

    connect(resetButton, &QPushButton::clicked, this, &LevelOnepa::on_ResetButton_clicked);
    // 创建回退按钮
    undoButton = new QPushButton("撤销", this);
    undoButton->setStyleSheet(buttonStyle);
    undoButton->setIcon(QIcon(":/new/images/cx.png"));
    undoButton->setIconSize(QSize(41, 41));
    connect(undoButton, &QPushButton::clicked, this, &LevelOnepa::undoLastOperation);

    submitButton = new QPushButton("提交", this);
    submitButton->setStyleSheet(buttonStyle);
    submitButton->setIcon(QIcon(":/new/images/tj.png"));
    submitButton->setIconSize(QSize(41, 41));
    connect(submitButton, &QPushButton::clicked, this, &LevelOnepa::on_SubmitButton_clicked);

    // 创建开始按钮
    startButton = new QPushButton("开始", this);
    startButton->setStyleSheet(buttonStyle);
    startButton->setIcon(QIcon(":/new/images/ks.png"));
    startButton->setIconSize(QSize(41, 41));
    connect(startButton, &QPushButton::clicked, this, &LevelOnepa::startGame);

    // 创建提示按钮
    QPushButton *hintButton = new QPushButton("提示", this);
    hintButton->setStyleSheet(buttonStyle);
    hintButton->setIcon(QIcon(":/new/images/tip.png"));
    hintButton->setIconSize(QSize(41, 41));
    connect(hintButton, &QPushButton::clicked, this, &::LevelOnepa::showhint);


    // 创建拼图
    puzzleCells = new QLabel*[puzzleSize * puzzleSize];
    puzzleGrid = new QGridLayout();
    puzzleGrid->setHorizontalSpacing(-1);
    puzzleGrid->setVerticalSpacing(-1);
    puzzleGrid->setContentsMargins(0, 0, 0, 0);

    for (int row = 0; row < puzzleSize; ++row)
    {
        for (int col = 0; col < puzzleSize; ++col)
        {
            QLabel *textCell = new QLabel();
            textCell->setFixedSize(70, 70);
            textCell->setStyleSheet(" font-weight: bold; color: white; font-size: 20pt; text-align: center; vertical-align: middle");
            textCell->setText(QString::number(0));

            // 创建特殊符号图片标签
            QLabel *symbolCell = new QLabel();
            symbolCell->setFixedSize(60, 60); // 2:1
            symbolCell->setAlignment(Qt::AlignCenter); // 图片居中显示
            // 设置初始特殊符号
            setSpecialSymbolForCell(symbolCell, hiddenMap[row * puzzleSize + col]);
            // 用一个布局将文本标签和特殊符号图片标签组合在一起
            QVBoxLayout *cellLayout = new QVBoxLayout();
            cellLayout->setContentsMargins(0, 0, 0, 0);
            symbolCell->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            cellLayout->insertWidget(0,symbolCell);
            cellLayout->insertWidget(1,textCell);
            // 将组合后的布局添加到拼图网格中
            QWidget *cellWrapper = new QWidget();
            cellWrapper->setLayout(cellLayout);
            puzzleCells[row * puzzleSize + col] = textCell;
            puzzleGrid->addWidget(cellWrapper, row, col);
        }
    }

    // 创建网格大小选择按钮
    gridSizeLayout = new QVBoxLayout();
    for (int i = 0; i < 4; ++i)
    {
        int gridSize =i+1; // 修改此处，使网格大小为 1x1、2x2、3x3、4x4

        QPushButton *button = new QPushButton(QString::number(gridSize) + "x" + QString::number(gridSize), this);
        button->setMinimumWidth(150); // 设置最小宽度为150
        button->setMaximumWidth(150); // 设置最大宽度为150
        button->setMinimumHeight(50); // 设置最小高度为50
        button->setMaximumHeight(50); // 设置最大高度为50
        connect(button, &QPushButton::clicked, this, [this, gridSize]() { on_GridSizeButton_clicked(gridSize); });
        // 设置按钮之间的间距
        gridSizeLayout->addSpacing(50); // 添加20像素的间距
        gridSizeButtons[i] = button;
        gridSizeButtons[i]->setStyleSheet(
            "QPushButton {"
            "background-color: transparent; border: 2px solid white; border-radius: 10px; "
            "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
            "}"
            "QPushButton:hover {"
            "color: lightgray;"  // 悬停时的颜色变化
            "}");
        gridSizeLayout->addWidget(button);
    }
    gridSizeLayout->setDirection(QVBoxLayout::BottomToTop);

    // 创建新的水平主布局
    mainLayout = new QHBoxLayout();

    // 左侧垂直布局，包含返回按钮和网格大小选择按钮
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(returnButton, 0, Qt::AlignLeft | Qt::AlignTop);
    leftLayout->setContentsMargins(0,0,0,400);
    //leftLayout->addWidget(returnButton, 0, Qt::AlignLeft | Qt::AlignTop);
    leftLayout->addSpacing(100);
    leftLayout->addWidget(timerLabel);  // 将计时器标签添加到左侧布局中
    leftLayout->addSpacing(100);
    leftLayout->addLayout(gridSizeLayout);

    // 中间布局，包含地图
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->addLayout(puzzleGrid); // 添加你的地图布局

    centerLayout->setContentsMargins(125, 0, 0, 400); // 设置左边距为50像素

    // 右侧垂直布局，包含开始、撤销、重置和提交按钮
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(100,100,0,400);

    rightLayout->addSpacing(10);  // 添加间距
    rightLayout->addWidget(startButton);
    rightLayout->addSpacing(10);  // 添加间距
    rightLayout->addWidget(undoButton);
    rightLayout->addSpacing(10);  // 添加间距
    rightLayout->addWidget(resetButton);
    rightLayout->addSpacing(10);  // 添加间距
    rightLayout->addWidget(submitButton);
    rightLayout->addSpacing(10);  // 添加间距
    rightLayout->addWidget(hintButton);
    rightLayout->addSpacing(10);

    // 设置布局之间的间距和对齐方式
    mainLayout->addStretch(1);  // 添加一个可伸缩的空间，使得布局在垂直方向上居中
    //
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addLayout(rightLayout);
    mainLayout->setContentsMargins(0, 0, 600, 0);
    connect(timer, SIGNAL(timeout()), this, SLOT()); // SLOT填入一个槽函数

    connect(timer, &QTimer::timeout, this, &LevelOnepa::updateTimer);

    // 设置计时器间隔为1秒
    timer->setInterval(1000); // 1000毫秒 = 1秒
    timer->stop(); // 启动计时器
    // 设置主布局为窗口布局
    this->setLayout(mainLayout);
    // 窗口设置
    // 设置窗口为全屏
    setWindowState(Qt::WindowFullScreen);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    // 背景设置
    QImage background(":/new/images/x.png");
    background = background.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(background));
    this->setPalette(palette);


}

void LevelOnepa::startGame()
{
    // 初始化计时器
    elapsedTimeSeconds = 0;
    timerLabel->setText("时间：0 秒");
    timer->start();

    // 初始化所有格子数值为0
    for (int i = 0; i < 36; ++i)
    {
        puzzleCells[i]->setText("0");
    }

    // 清空所有按钮使用记录
    for (int i = 0; i < useCounts.size(); ++i)
    {
        useCounts[i] = 0;
    }
    // 清空撤销栈
    undoStack.clear();

    // 初始化其他相关状态变量
    isButtonClicked = false;
}

LevelOnepa::~LevelOnepa()
{
    delete[] puzzleCells;
}

void LevelOnepa::on_ReturnButton_clicked()
{
    emit returnToPracticePage(); // 发出返回练习页面信号
}

void LevelOnepa::on_PuzzleCell_clicked(QLabel *cell, int clickedRow, int clickedCol)
{
    if (!isButtonClicked)
    {
        return; // 若未点击按钮，则不处理格子点击事件，直接返回
    }

    int bottomRow = clickedRow + currentGridSize - 1;
    int rightCol = clickedCol + currentGridSize - 1;

    if (bottomRow >= puzzleSize || rightCol >= puzzleSize)
    {
        CustomMessageBox messageBox(tr("您选择的区域超过了拼图边界！"));
        if (messageBox.exec() == QDialog::Accepted) {
            return;
        }
    }
    if (currentGridSize == 1) // 修正：当使用1x1按钮时，直接更新单个格子的文本
    {
        cell->setText(QString::number(cell->text().toInt() + 1));
    }
    else
    {
        for (int row = clickedRow; row < std::min(clickedRow + currentGridSize, 6); ++row)
        {
            for (int col = clickedCol; col < std::min(clickedCol + currentGridSize, 6); ++col)
            {
                QLabel *targetCell = puzzleCells[row * puzzleSize + col];
                targetCell->setText(QString::number(targetCell->text().toInt() + 1));
            }
        }
    }
    isButtonClicked=false;//需要重置一下

    useCounts[currentGridSize]++; // 递增当前按钮的计数器
    // 记录当前操作信息
    QString operationDescription = QStringLiteral("%1,%2,%3").arg(clickedRow).arg(clickedCol).arg(currentGridSize);
    undoStack.push(operationDescription);
}

void LevelOnepa::on_GridSizeButton_clicked(int size)
{
    bool allButtonsUsed = true;
    for (int i = 1; i <= 4; ++i)
    {
        if (useCounts[i] < maxUseCounts[i])
        {
            allButtonsUsed = false;
            break;
        }
    }
    bool puzzleMatchesHidden = comparePuzzleWithHiddenMap();
    if(allButtonsUsed && puzzleMatchesHidden)
    {
        timer->stop();
        isGameFinished = true;
        showSuccessDialog(); // 显示成功对话框
        setVictory(1);
        return ;
    }
    else if(allButtonsUsed && !puzzleMatchesHidden)
    {
        timer->stop();
        isGameFinished = true;
        showFailureDialog(); // 显示失败对话框
        setVictory(2);
        return;
    }

    if (useCounts[size] >= maxUseCounts[size ])
    {
        return; // 若已达最大使用次数，则忽略此次点击事件
    }

    currentGridSize = size;

    isButtonClicked = true; // 设置标志变量为 true，表示已点击按钮

}

void LevelOnepa::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    // 获取鼠标点击在 puzzleGrid 中的位置
    QPoint gridPos = event->pos();

    // 检查 puzzleGrid 是否有效
    if (!puzzleGrid)
    {
        qWarning("puzzleGrid is null");
        return;
    }

    // 检查点击位置是否在 puzzleGrid 区域内
    if (!puzzleGrid->parentWidget()->rect().contains(gridPos))
    {
        return; // 点击位置不在 puzzleGrid 内，直接返回
    }

    // 转换为相对于 puzzleGrid 左上角的坐标
    gridPos -= puzzleGrid->geometry().topLeft();

    // 根据 puzzleSize 计算实际点击的单元格索引
    int cellWidth = puzzleGrid->geometry().width() / puzzleSize;
    int cellHeight = puzzleGrid->geometry().height() / puzzleSize;
    int clickedRow = gridPos.y() / cellHeight;
    int clickedCol = gridPos.x() / cellWidth;

    // 检查计算出的索引是否在有效范围内
    if (clickedRow < 0 || clickedRow >= puzzleSize || clickedCol < 0 || clickedCol >= puzzleSize)
    {
        qWarning("Clicked position out of grid bounds: row=%d, col=%d", clickedRow, clickedCol);
        return;
    }

    // 通过索引获取对应的单元格（cellWrapper）
    QLayoutItem *item = puzzleGrid->itemAt(clickedRow * puzzleSize + clickedCol);
    if (!item)
    {
        qWarning("No item at clicked position: row=%d, col=%d", clickedRow, clickedCol);
        return;
    }

    QWidget *cellWrapper = item->widget();
    if (!cellWrapper)
    {
        qWarning("cellWrapper is null at clicked position: row=%d, col=%d", clickedRow, clickedCol);
        return;
    }

    QLabel *label = nullptr;

    // 遍历点击的 QWidget（cellWrapper）中的所有 QLabel，找到被点击的那个
    for (QObject *child : cellWrapper->children())
    {
        label = qobject_cast<QLabel *>(child);
        if (label && label->underMouse())
        {
            break;
        }
    }

    if (label)
    {
        on_PuzzleCell_clicked(label, clickedRow, clickedCol);
        bool allButtonsUsed = true;
        for (int i = 1; i <= 4; ++i)
        {
            if (useCounts[i] < maxUseCounts[i])
            {
                allButtonsUsed = false;
                break;
            }
        }
        bool puzzleMatchesHidden = comparePuzzleWithHiddenMap();
        if(allButtonsUsed && puzzleMatchesHidden)
        {
            isGameFinished = true;
            timer->stop();
            showSuccessDialog(); // 显示成功对话框
            setVictory(1);
            return;
        }
        else if(allButtonsUsed && !puzzleMatchesHidden)
        {
            isGameFinished = true;
            timer->stop();
            showFailureDialog(); // 显示失败对话框
            setVictory(2);
            return;
        }
    }
    else
    {
        qWarning("No QLabel under mouse at clicked position: row=%d, col=%d", clickedRow, clickedCol);
    }
}


bool LevelOnepa::comparePuzzleWithHiddenMap()
{
    for (int row = 0; row < puzzleSize; ++row)
    {
        for (int col = 0; col < puzzleSize; ++col)
        {
            QLabel *cell = puzzleCells[row * puzzleSize + col];
            int playerValue = cell->text().toInt();
            int hiddenValue = hiddenMap[row * puzzleSize + col];
            if (hiddenValue == 2 && playerValue != 0 && playerValue != 2)
            {
                return false; // 当隐藏地图格子为2时，玩家地图格子不是0或2，则返回 false
            }
            else if (hiddenValue == 0 && playerValue != 0)
            {
                return false; // 当隐藏地图格子为0时，玩家地图格子不是0，则返回 false
            }
            else if (hiddenValue == 4 && playerValue != 4)
            {
                return false; // 当隐藏地图格子为4时，玩家地图格子不是4，则返回 false
            }
            else if (hiddenValue == 1 && playerValue != 1 && playerValue != 3)
            {
                return  false; // 当隐藏地图格子为1时，玩家地图格子不是1或3，则返回 false
            }
        }
    }
    return true; // 所有条件均满足，返回 true
}


void LevelOnepa::showSuccessDialog()
{
    SuccessDialog dialog(this);
    dialog.setFixedWidth(600); // 将宽度从 300 像素增加到 400 像素
    dialog.setFixedHeight(650); // 将高度从 100 像素增加到 150 像素
    dialog.exec();
}

void LevelOnepa::showFailureDialog()
{
    FailureDialog dialog(this);
    dialog.setFixedWidth(600); // 将宽度从 300 像素增加到 400 像素
    dialog.setFixedHeight(650); // 将高度从 100 像素增加到 150 像素
    dialog.exec();
}


void LevelOnepa::undoLastOperation()
{
    if (undoStack.isEmpty())
        return; // 如果没有可撤销的操作，直接返回
    // 获取上一步操作信息
    QString lastOperation = undoStack.pop();

    // 解析操作信息
    QStringList operationParts = lastOperation.split(",");
    if (operationParts.size() != 3)
        return; // 操作信息格式错误，无法撤销
    int clickedRow = operationParts[0].toInt();
    int clickedCol = operationParts[1].toInt();
    int gridSize = operationParts[2].toInt();

    // 撤销上一步操作：将相应区域内的格子数值减一
    QVector<QLabel*> targetCells;
    for (int row = clickedRow; row < std::min(clickedRow + gridSize, puzzleSize); ++row)
    {
        for (int col = clickedCol; col < std::min(clickedCol + gridSize, puzzleSize); ++col)
        {
            targetCells.append(puzzleCells[row * puzzleSize + col]);
        }
    }

    for (auto targetCell : targetCells)
    {
        int currentValue = targetCell->text().toInt();
        if (currentValue > 0)
        {
            targetCell->setText(QString::number(currentValue - 1));
        }
    }

    // 更新计数器
    useCounts[currentGridSize]--;
    if (useCounts[currentGridSize] < maxUseCounts[currentGridSize])
    {
        isButtonClicked = false; // 只有当计数器减至未达最大使用次数时，才允许再次点击按钮
    }
}


void LevelOnepa::on_ResetButton_clicked()
{
    // 重置所有格子数值为0
    for (int i = 0; i < 36; ++i) {
        puzzleCells[i]->setText("0");
    }

    // 清空所有按钮使用记录
    for (int i = 0; i < useCounts.size(); ++i)
    {
        useCounts[i] = 0;
    }
    // 清空撤销栈
    undoStack.clear();

    // 根据需要，可重置其他相关状态变量（如isButtonClicked等）
    isButtonClicked = false;

    elapsedTimeSeconds = 0;
    timerLabel->setText("时间：0 秒");
    timer->stop(); // 重新启动计时器
}

// 新增方法：根据隐藏地图值设置对应格子的特殊符号
void LevelOnepa::setSpecialSymbolForCell(QLabel *cell, int hiddenValue)
{
    // 加载特殊符号图片
    QString imagePath = symbolFilePaths.value(hiddenValue);
    QPixmap symbolPixmap(imagePath);
    QPixmap picture1 ;
    // 检查pixmap是否有效
    cell->setPixmap(symbolPixmap);
    cell->setScaledContents(true);//自适应大小
}

void LevelOnepa::readHiddenMapFromFile(const QString &filePath, std::vector<int> &hiddenMap)
{

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open file" << filePath;
        return;
    }
    QTextStream in(&file);
    for (int i = 0; i < 36; ++i)
    {
        bool ok;
        int value = in.readLine().trimmed().toInt(&ok);
        if (!ok)
        {
            qWarning() << "Invalid value encountered while reading file" << filePath;
            return;

        }
        hiddenMap[i] = value;
    }
    file.close();
}

void LevelOnepa::updateTimer()
{
    elapsedTimeSeconds++;
    timerLabel->setText(QString("时间：%1 秒").arg(elapsedTimeSeconds));
}

void LevelOnepa::on_SubmitButton_clicked()
{
    // 重置所有格子数值为初始状态
    if (!isGameFinished||!comparePuzzleWithHiddenMap())
    {
        CustomMessageBox messageBox("游戏还未结束或您未取得胜利，不能提交！", this);
        messageBox.exec();
        return;
    }

    // 弹出对话框让用户输入用户名
    QString userName =_username;
    // 获取时间
    QString elapsedTimeString = QString::number(elapsedTimeSeconds) ;

    // 构建提交信息
    QString submitInfo = QString("%1 %2 %3 %4").arg(_nowstate).arg(userName).arg(elapsedTimeString).arg(imagePath);

    // 写入文件
    QString filePath = "./www.txt"; // 文件路径
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << submitInfo << "\n"; // 每次写入一行
        file.close();
    }
    else
    {
        QMessageBox::warning(this, "错误", "无法打开文件进行写入！");
        return;
    }

    for (int i = 0; i < 36; ++i) {
        puzzleCells[i]->setText("0");
    }

    // 清空所有按钮使用记录
    for (int i = 0; i < useCounts.size(); ++i)
    {
        useCounts[i] = 0;
    }
    // 清空撤销栈
    undoStack.clear();

    isButtonClicked = false;

    // 重置计时器
    elapsedTimeSeconds = 0;
    timerLabel->setText("时间：0 秒");
    timer->start(); // 重新启动计时器
    CustomMessageBox mmessageBox("您已成功提交!");
    mmessageBox.exec();
    on_ResetButton_clicked();
}

void LevelOnepa::update(const QString &filePath)
{
    //删除之间的控件
    QLayoutItem *child;
    while ((child = puzzleGrid->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    readHiddenMapFromFile(filePath,hiddenMap);
    //读取地图，然后直接进行更新

    for (int row = 0; row < puzzleSize; ++row)
    {
        for (int col = 0; col < puzzleSize; ++col)
        {
            QLabel *textCell = new QLabel(this);
            textCell->setFixedSize(70, 70); // 示例尺寸，根据实际需求调整
            textCell->setStyleSheet("font-weight: bold; color: white; font-size: 20pt; text-align: center; vertical-align: middle");
            textCell->setText(QString::number(0));

            // 创建特殊符号图片标签
            QLabel *symbolCell = new QLabel(this);
            symbolCell->setFixedSize(60, 60); // 2:1 // 示例尺寸，根据实际需求调整
            symbolCell->setAlignment(Qt::AlignCenter); // 图片居中显示

            // 设置初始特殊符号
            setSpecialSymbolForCell(symbolCell, hiddenMap[row * puzzleSize + col]);

            // 用一个布局将文本标签和特殊符号图片标签组合在一起
            QVBoxLayout *cellLayout = new QVBoxLayout();
            cellLayout->setContentsMargins(0, 0, 0, 0);
            symbolCell->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            cellLayout->insertWidget(0,symbolCell);
            cellLayout->insertWidget(1,textCell);
            // 将组合后的布局添加到拼图网格中
            QWidget *cellWrapper = new QWidget(this);
            cellWrapper->setLayout(cellLayout);
            puzzleCells[row * puzzleSize + col] = textCell;
            puzzleGrid->addWidget(cellWrapper, row, col);
        }
    }
}

void LevelOnepa::upset()
{
    hiddenMap.resize(36);
}


void LevelOnepa::showhint()
{
    auto results = getans();
    QString resultStr;
    if (!results.empty())
    {
        for (const auto& x : results)
        {
            resultStr += QString("(%1, %2): %3\n").arg(x.first.first).arg(x.first.second).arg(x.second);
        }
    }
    else
    {
        resultStr = "no solution";
    }

    // 创建自定义提示对话框
    CustomHintDialog hintDialog(this);
    hintDialog.setResultText(resultStr + "\n1*1的格子就看你自己了，\n作者很努力的写出了这个判断解，你就动动手罢！");

    // 显示对话框
    hintDialog.exec();
}

int maze2[10][10];
int hiden2[10][10];
//这下就判断好了
//判断地图有没有解的问题，并给予用户提示
std::vector<std::pair<std::pair<int,int> ,int>> LevelOnepa::getans()
{
    std::pair<int,int>a[30]={{0,0},{1,1},{1,2},{1,3},{2,1},{2,2},{2,3},{3,1},{3,2},{3,3}};
    std::pair<int,int>b[30]={{0,0},{1,1},{1,2},{1,3},{1,4},{2,1},{2,2},{2,3},{2,4},{3,1},{3,2},{3,3},{3,4},{4,1},{4,2},{4,3},{4,4}};
    std::pair<int,int>c[30]={{0,0},{1,1},{1,2},{1,3},{1,4},{1,5},{2,1},{2,2},{2,3},{2,4},{2,5},{3,1},{3,2},{3,3},{3,4},{3,5},{4,1},{4,2},{4,3},{4,4},{4,5},{5,1},{5,2},{5,3},{5,4},{5,5}};
    std::vector<std::pair<std::pair<int,int> ,int>>ans;
    auto ok = [&](int i, int j) -> bool
    {
        if(hiden2[i][j]==0)
        {
            if(maze2[i][j]!=0)
            {
                return false;
            }
        }
        else if(hiden2[i][j]==2)
        {
            if(maze2[i][j]==1||maze2[i][j]==3||maze2[i][j]==4)
            {
                return false;
            }
        }
        else if(hiden2[i][j]==1)
        {
            if(maze2[i][j]==0||maze2[i][j]==2||maze2[i][j]==4)
            {
                return false;
            }
        }
        else if(hiden2[i][j]==4)
        {
            if(maze2[i][j]!=4)
            {
                return false;
            }
        }
        return true;
    };

    auto add = [&](int x1, int y1, int x2, int y2, int c) {
        // x1-=1;
        // y1-=1;
        // x2-=1;
        // y2-=1;
        maze2[x1][y1] += c;
        maze2[x1][y2+1] -=c;
        maze2[x2+1][y1] -=c;
        maze2[x2+1][y2+1] += c;
    };

    auto add_ = [&]() -> bool
    {
        int cnt=0;
        for (int i=1; i<=6; i++)
        {
            for (int j=1; j<=6; j++)
            {
                maze2[i][j] += maze2[i-1][j]+maze2[i][j-1]-maze2[i-1][j-1];
                if(maze2[i][j]>4)
                {
                    return false;
                }
            }
        }
        for (int i=1; i<=6; i++)
        {
            for (int j=1; j<=6; j++)
            {
                if(hiden2[i][j]==1&&(maze2[i][j]==0||maze2[i][j]==2))
                {
                    cnt++;
                    // cout<<'x'<<'\n';
                    // cout<<i<<' '<<j<<'\n';
                    maze2[i][j]++;
                }
                if( hiden2[i][j]==2&&maze2[i][j]==1)
                {
                    cnt++;
                    //     cout<<'y'<<'\n';
                    // cout<<i<<' '<<j<<'\n';
                    maze2[i][j]++;
                }
                if(hiden2[i][j]==4)
                {
                    cnt+=4-maze2[i][j];
                }
                if(cnt>=3)
                {
                    return false;
                }
                if(!ok(i,j))
                {
                    return false;
                }
            }
        }
        return true;
    };

    //直接赋值给隐藏的那一块地图
    for (int i = 0; i <= 35; i++)
    {
        int x = i / 6 + 1;
        int y = i % 6 + 1;
        hiden2[x][y] = hiddenMap[i];
        qDebug()<<hiden2[x][y];
    }

    for(int i=1;i<=9;i++)
    {
        for(int j=1;j<=16;j++)
        {
            for(int k=1;k<=16;k++)
            {
                if(j==k)
                {
                    continue;
                }
                for(int h=1;h<=25;h++)
                {
                    for(int f=1;f<=25;f++)
                    {
                        if(h==f)
                        {
                            continue;
                        }
                        //然后就是求解一个二维度前缀和即可
                        add(a[i].first,a[i].second,a[i].first+3,a[i].second+3,1);
                        add(b[j].first,b[j].second,b[j].first+2,b[j].second+2,1);
                        add(b[k].first,b[k].second,b[k].first+2,b[k].second+2,1);
                        add(c[h].first,c[h].second,c[h].first+1,c[h].second+1,1);
                        add(c[f].first,c[f].second,c[f].first+1,c[f].second+1,1);
                        if(add_())
                        {
                            ans.push_back({a[i],4});
                            ans.push_back({b[j],3});
                            ans.push_back({b[k],3});
                            ans.push_back({c[h],2});
                            ans.push_back({c[f],2});
                            f=26,h=26;k=17;j=17;i=10;
                        }


                        for(int l=0;l<=35;l++)
                        {
                            int x=l/6+1;
                            int y=l%6+1;
                            maze2[x][y]=0;
                        }
                    }
                }
            }
        }
    }
    if(ans.size()!=0)
    {
        for(auto x:ans)
        {
            qDebug()<<x.first.first<<' '<<x.first.second<<":"<<' '<<x.second<<'\n';
        }
    }
    else
    {
        qDebug()<<"no solution";
    }
    return ans;
}

//最大的特点就是有一个胜负状态信号

void LevelOnepa::setVictory(int value)
{
    victory = value;
    qDebug()<<"win";
    emit gameEnded();  // 发出胜负状态信号
}


void LevelOnepa::setShadow()
{
}




