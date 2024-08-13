#include "FreeModePage.h"
#include "CompetitionPage.h"
#include"SuccessDialog.h"
#include"FailureDialog.h"
#include <QRandomGenerator>
#include<QPalette>
#include<QMessageBox>
#include<QFile>
#include<QInputDialog>
#include<QFile>
#include "CustomHintDialog.h"
#include "custommessagebox.h"
FreeModePage::FreeModePage(CompetitionPage* parent)
    : QWidget(parent)
{
    std::srand(std::time(nullptr));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 10000000);
    const int mapSize = 6;
    //0-35的一个一维度向量
    std::vector<int> map(mapSize * mapSize, 0);
    //随机算法是通过定义了一下地图切片结构体实现的，由于地图的属性问题
    //第一个变量是类型，用枚举，刚好是0，1，2，3
    //第二个变量是值，也就是格子加的值
    //第三个变量是x坐标
    //第四个是y坐标
    std::vector<Puzzle> puzzles =
        {
            Puzzle(PT_1x1, 1, 0, 0),
            Puzzle(PT_1x1, 1, 0, 0),
            Puzzle(PT_2x2, 1, 0, 0),
            Puzzle(PT_2x2, 1, 0, 0),
            Puzzle(PT_3x3, 1, 0, 0),
            Puzzle(PT_3x3, 1, 0, 0),
            Puzzle(PT_4x4, 1, 0, 0)
        };
    //地图算法主要是随机，采用梅森随机数和rand生成满足题意得位置坐标
    //并开始循环放置，知道满足地图条件为止
    //这里的地图条件就是大于4的至少有一个，并加大难度，设置0个也就是×的为圆形，加大了难度
    while(1)
    {
        for (int i = 0; i <= 35; i++)
        {
            map[i] = 0;
        }
        int placed = 0;
        for (Puzzle& puzzle : puzzles)
        {
            puzzle.x = (distrib(gen)) % (6 - puzzle.type);
            puzzle.y = (distrib(gen)) % (6 - puzzle.type);
            qDebug()<<puzzle.x<<puzzle.y;
            if (puzzle.type == 0)
            {
                map[puzzle.x * 6 + puzzle.y]++;
                qDebug()<<"iii"<<'\n';
                continue;
            }
            if (puzzle.x + puzzle.type+1 > 6 || puzzle.y + puzzle.type+1 > 6)
            {
                placed = 1;
                for (int i = 0; i <= 35; i++)
                {
                    map[i] = 0;
                }
                break;
            }
            //0,0,1,1,0,1,6,7
            for (int dx = puzzle.x; dx <= puzzle.type + puzzle.x; ++dx)
            {
                for (int dy = puzzle.y; dy <= puzzle.type + puzzle.y; ++dy)
                {
                    int index = dx * 6 +dy;
                    map[index]++;
                }
            }
        }
        if (placed)
        {
            continue;
        }
        int flag = 0;
        int cnt=0;
        for (int i = 0; i <= 35; i++)
        {
            if(map[i]==4)
            {
                cnt++;
            }
            if (map[i] > 4)
            {
                flag = 1;
                break;
            }
        }
        if (flag||cnt<1)
        {
            for (int i = 0; i <= 35; i++)
            {
                map[i] = 0;
            }
            continue;
        }
        else
        {
            break;
        }
    }
//对即将存入文件的地图进行预处理
    for(int i=0;i<=35;i++)
    {
        if(map[i]==0)
        {
            map[i]=2;
        }
        //0->2,3->1
        if(map[i]==3)
        {
            map[i]=1;
        }
    }
    //以上方法就是随机生成位置然后进行放置，然后进行判断，一定要有至少一个4，并且不能大于4，随机地图的生成时符合正常逻辑
    //然后存放进地图，这就是随机算法
    QFile file("./xxx.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (int val : map)
        {
            out << val << '\n';
        }
        file.close();
    }
    //安全性
    else
    {
        qWarning() << "Failed to open file for writing";
    }
    //直接以只读方式打开文件进行读取

    QVBoxLayout *vLayout = new QVBoxLayout();
    timerLabel = new QLabel("时间：0 秒", this);
    timerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white; border: 2px solid black; padding: 5px;");
    // 初始化隐藏地图数据
    hiddenMap.resize(36);
    readHiddenMapFromFile(QStringLiteral("./xxx.txt"), hiddenMap);
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

    //以上为初始化代码，每个按钮的最大点击次数需要进行初始化
    returnButton = new QPushButton("返回", this);
    // 创建并设置关卡按钮
    QString buttonStyle =(   "QPushButton {"
                           "background-color: transparent; border: 2px solid white; border-radius: 10px; "
                           "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                           "}"
                           "QPushButton:hover {"
                           "color: lightgray;"  // 悬停时的颜色变化
                           "}");
    returnButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");
    connect(returnButton, &QPushButton::clicked, this, &FreeModePage::returnToCompetitionPage);

    resetButton = new QPushButton("重置", this);
    resetButton->setStyleSheet(
        "font-family: \"Microsoft YaHei\"; font-size: 28px; font-weight: bold; color: white; "
        "border-radius: 8px; padding: 15px; min-width: 60px; min-height: 30px;");
    resetButton->setStyleSheet(buttonStyle);
    resetButton->setIcon(QIcon(":/new/images/cxks.png"));
    resetButton->setIconSize(QSize(41, 41));
    connect(resetButton, &QPushButton::clicked, this, &FreeModePage::on_ResetButton_clicked);

    // 创建回退按钮
    undoButton = new QPushButton("撤销", this);
    undoButton->setStyleSheet(buttonStyle);
    undoButton->setIcon(QIcon(":/new/images/cx.png"));
    undoButton->setIconSize(QSize(41, 41));
    connect(undoButton, &QPushButton::clicked, this, &FreeModePage::undoLastOperation);

    //创建提交按钮
    submitButton = new QPushButton("提交", this);
    submitButton->setStyleSheet(buttonStyle);
    submitButton->setIcon(QIcon(":/new/images/tj.png"));
    submitButton->setIconSize(QSize(41, 41));
    connect(submitButton, &QPushButton::clicked, this, &FreeModePage::on_SubmitButton_clicked);

    // 创建开始按钮
    startButton = new QPushButton("开始", this);
    startButton->setStyleSheet(buttonStyle);
    startButton->setIcon(QIcon(":/new/images/ks.png"));
    startButton->setIconSize(QSize(41, 41));
    connect(startButton, &QPushButton::clicked, this, &FreeModePage::startGame);

    // 创建提示按钮
    QPushButton *hintButton = new QPushButton("提示", this);
    hintButton->setStyleSheet(buttonStyle);
    hintButton->setIcon(QIcon(":/new/images/tip.png"));
    hintButton->setIconSize(QSize(41, 41));
    connect(hintButton, &QPushButton::clicked, this, &FreeModePage::showhint);

    // 创建提示按钮
    QPushButton *initButton = new QPushButton("再来一副", this);
    initButton->setStyleSheet(buttonStyle);
    initButton->setIcon(QIcon(":/new/images/tip.png"));
    initButton->setIconSize(QSize(41, 41));
    connect(initButton, &QPushButton::clicked, this, &FreeModePage::initMap);

    //以上都是按钮的创建和槽函数的连接

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
            QLabel *textCell = new QLabel(this);
            textCell->setFixedSize(70, 70);
            textCell->setStyleSheet("font-weight: bold; color: white; font-size: 20pt; text-align: center; vertical-align: middle");
            textCell->setText(QString::number(0));

            //文字标签

            // 创建特殊符号图片标签
            QLabel *symbolCell = new QLabel(this);
            symbolCell->setFixedSize(60, 60);
            symbolCell->setAlignment(Qt::AlignCenter);

            //图片标签

            // 设置初始特殊符号，根据符号标签，和对应的文字设置图片
            setSpecialSymbolForCell(symbolCell, hiddenMap[row * puzzleSize + col]);

            // 用一个布局将文本标签和特殊符号图片标签组合在一起
            QVBoxLayout *cellLayout = new QVBoxLayout();
            cellLayout->setContentsMargins(0, 0, 0, 0);
            symbolCell->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            cellLayout->insertWidget(0,symbolCell);
            cellLayout->insertWidget(1,textCell);
            //图片在上面，文字在下面

            // 将组合后的布局添加到拼图网格中
            QWidget *cellWrapper = new QWidget(this);
            cellWrapper->setLayout(cellLayout);
            puzzleCells[row * puzzleSize + col] = textCell;
            //地图标签存储问文本
            puzzleGrid->addWidget(cellWrapper, row, col);
            //同时拼图网格布局存储一个用来存放图片和文字的QWidget
        }
    }
    gridSizeLayout = new QVBoxLayout();
    //作为右侧的按钮布局
    for (int i = 0; i < 4; ++i)
    {
        int gridSize =i+1;
        QPushButton *button = new QPushButton(QString::number(gridSize) + "x" + QString::number(gridSize), this);
        button->setMinimumWidth(150);
        button->setMaximumWidth(150);
        button->setMinimumHeight(50);
        button->setMaximumHeight(50);
        //连接1*1，2*2，3*3，4*4按钮到槽函数
        connect(button, &QPushButton::clicked, this, [this, gridSize]() { on_GridSizeButton_clicked(gridSize); });
        gridSizeLayout->addSpacing(50);
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
    //创建4个按钮，并且连接各自的槽函数，结合了题目的大小
    gridSizeLayout->setDirection(QVBoxLayout::BottomToTop);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左侧垂直布局，包含返回按钮和网格大小选择按钮
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0,0,0,400);
    leftLayout->addWidget(returnButton, 0, Qt::AlignLeft | Qt::AlignTop);
    leftLayout->addSpacing(100);
    leftLayout->addWidget(timerLabel);  // 将计时器标签添加到左侧布局中
    leftLayout->addSpacing(100);
    leftLayout->addLayout(gridSizeLayout);

    // 中间布局，包含地图
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->addLayout(puzzleGrid);
    centerLayout->addWidget(timerLabel);

    // 设置左侧布局的左边距
    centerLayout->setContentsMargins(125, 0, 0, 0); // 设置左边距为50像素

    // 右侧垂直布局，包含开始、撤销、重置和提交按钮
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(100,100,0,300);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(startButton);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(undoButton);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(resetButton);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(submitButton);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(hintButton);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(initButton);
    rightLayout->addSpacing(10);

    // 设置布局之间的间距和对齐方式
    mainLayout->addStretch(1);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addLayout(rightLayout);

    // 设置主布局
    this->setLayout(mainLayout);

    connect(timer, SIGNAL(timeout()), this, SLOT());
    connect(timer, &QTimer::timeout, this, &FreeModePage::updateTimer);
    //时间
    // 设置计时器间隔为1秒
    timer->setInterval(1000);

    timer->stop(); // 启动计时器
}

void FreeModePage::startGame()
{
    // 初始化计时器
    elapsedTimeSeconds = 0;
    timerLabel->setText("时间：0 秒");
    timer->start();
    // 初始化隐藏地图
    readHiddenMapFromFile(QStringLiteral("./xxx.txt"), hiddenMap);
    // 初始化所有格子数值为0
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
    // 初始化其他相关状态变量
    isButtonClicked = false;
}


void FreeModePage::on_ReturnButton_clicked()
{
    emit returnToCompetitionPage();
}


void FreeModePage::on_PuzzleCell_clicked(QLabel *cell, int clickedRow, int clickedCol)
{
    if (!isButtonClicked)
    {
        return; // 若未点击按钮，则不处理格子点击事件，直接返回
    }

    int bottomRow = clickedRow + currentGridSize - 1;
    int rightCol = clickedCol + currentGridSize - 1;

    if (bottomRow >= puzzleSize || rightCol >= puzzleSize)
    {
        // 点击区域超出了拼图边界，此次操作无效
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
    useCounts[currentGridSize]++; // 递增当前按钮的计数器
    isButtonClicked=false;
    QString operationDescription = QStringLiteral("%1,%2,%3").arg(clickedRow).arg(clickedCol).arg(currentGridSize);
    //放入步骤
    undoStack.push(operationDescription);
}

void FreeModePage::on_GridSizeButton_clicked(int size)
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
    //如果按钮还没有被点击完，直接退出
    bool puzzleMatchesHidden = comparePuzzleWithHiddenMap();
    //看一下是否满足游戏结束条件
    if(allButtonsUsed && puzzleMatchesHidden)
    {
        isGameFinished = true;
        showSuccessDialog(); // 显示成功对话框
        timer->stop();
            on_ResetButton_clicked();
    }
    else if(allButtonsUsed && !puzzleMatchesHidden)
    {
        isGameFinished = true;
        showFailureDialog(); // 显示失败对话框
        timer->stop();
            on_ResetButton_clicked();
    }
    //以上时游戏胜利的判断

    if (useCounts[size] >= maxUseCounts[size ])
    {
        return;
        // 若已达最大使用次数，则忽略此次点击事件
    }

    currentGridSize = size;
    //记录当前的大小
    isButtonClicked = true;
    // 设置标志变量为 true，表示已点击按钮
}

void FreeModePage::mousePressEvent(QMouseEvent *event)
{
    //一定要是左键才可以
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
    //通过计算得到行和列

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
        //点击事件
        on_PuzzleCell_clicked(label, clickedRow, clickedCol);
        //点击完判断一下胜负
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
            showSuccessDialog(); // 显示成功对话框   
            timer->stop();
            on_ResetButton_clicked();
        }
        else if(allButtonsUsed && !puzzleMatchesHidden)
        {
            isGameFinished = true;
            showFailureDialog(); // 显示失败对话框
            timer->stop();
            on_ResetButton_clicked();
        }
        //判断成功
    }
    else
    {
        qWarning("No QLabel under mouse at clicked position: row=%d, col=%d", clickedRow, clickedCol);
    }
}

//得到标签，然后将文本转化为int，直接进行比较
bool FreeModePage::comparePuzzleWithHiddenMap()
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
            else if (hiddenValue == 1 && playerValue != 1 && playerValue != 3) {
                return  false; // 当隐藏地图格子为1时，玩家地图格子不是1或3，则返回 false
            }
        }
    }
    return true; // 所有条件均满足，返回 true
}

//展示成功界面
void FreeModePage::showSuccessDialog()
{
    SuccessDialog dialog(this);
    dialog.setFixedWidth(600); // 将宽度从 300 像素增加到 400 像素
    dialog.setFixedHeight(650); // 将高度从 100 像素增加到 150 像素
    dialog.exec();
}

//展示失败页面
void FreeModePage::showFailureDialog()
{
    FailureDialog dialog(this);
    dialog.setFixedWidth(600); // 将宽度从 300 像素增加到 400 像素
    dialog.setFixedHeight(650); // 将高度从 100 像素增加到 150 像素
    dialog.exec();
}

//撤回操作
void FreeModePage::undoLastOperation()
{
    if (undoStack.isEmpty())
        return;

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
    //先拿到所有的各自标签，以便进行修改

    for (auto targetCell : targetCells)
    {
        int currentValue = targetCell->text().toInt();
        if (currentValue > 0)
        {
            targetCell->setText(QString::number(currentValue - 1));
        }
    }
    //取出来全部减去1

    // 更新计数器
    useCounts[currentGridSize]--;
    if (useCounts[currentGridSize] < maxUseCounts[currentGridSize])
    {
        isButtonClicked = false; // 只有当计数器减至未达最大使用次数时，才允许再次点击按钮
    }
}


void FreeModePage::on_ResetButton_clicked()
{
    // 重置所有格子数值为0
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

    isButtonClicked = false;

    elapsedTimeSeconds = 0;
    timerLabel->setText("时间：0 秒");
    timer->stop(); // 重新启动计时器
}

// 新增方法：根据隐藏地图值设置对应格子的特殊符号
void FreeModePage::setSpecialSymbolForCell(QLabel *cell, int hiddenValue)
{
    // 加载特殊符号图片
    QString imagePath = symbolFilePaths.value(hiddenValue);
    QPixmap symbolPixmap(imagePath);
    QPixmap picture1 ;
    cell->setPixmap(symbolPixmap);
    cell->setScaledContents(true);//自适应大小
    //导入图片，根据之前的一个map进行操作哟
}

//从文件中读取隐藏地图
void FreeModePage::readHiddenMapFromFile(const QString &filePath, std::vector<int> &hiddenMap)
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

//更新计时器
void FreeModePage::updateTimer()
{
    elapsedTimeSeconds++;
    timerLabel->setText(QString("时间：%1 秒").arg(elapsedTimeSeconds));
}


void FreeModePage::on_SubmitButton_clicked()
{
    if (!isGameFinished||!comparePuzzleWithHiddenMap())
    {
        CustomMessageBox messageBox("不能提交，虚名乃是浮云啊少年！", this);
        messageBox.exec();
        return;
    }
}


void FreeModePage::update(const QString &filePath)
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

void FreeModePage::upset()
{
    hiddenMap.resize(36);
}

void FreeModePage::showhint()
{
    auto results = getans();
    QString resultStr;
    if (!results.empty()) {
        for (const auto& x : results) {
            resultStr += QString("(%1, %2): %3\n").arg(x.first.first).arg(x.first.second).arg(x.second);
        }
    }
    else {
        resultStr = "no solution";
    }

    // 创建自定义提示对话框
    CustomHintDialog hintDialog(this);
    hintDialog.setResultText(resultStr + "\n1*1的格子就看你自己了，\n作者很努力的写出了这个判断解，你就动动手罢！");

    // 显示对话框
    hintDialog.exec();
}
int maze1[10][10];
int hiden1[10][10];
//这下就判断好了
//判断地图有没有解的问题，并给予用户提示
std::vector<std::pair<std::pair<int,int> ,int>> FreeModePage::getans()
{
    std::pair<int,int>a[30]={{0,0},{1,1},{1,2},{1,3},{2,1},{2,2},{2,3},{3,1},{3,2},{3,3}};
    std::pair<int,int>b[30]={{0,0},{1,1},{1,2},{1,3},{1,4},{2,1},{2,2},{2,3},{2,4},{3,1},{3,2},{3,3},{3,4},{4,1},{4,2},{4,3},{4,4}};
    std::pair<int,int>c[30]={{0,0},{1,1},{1,2},{1,3},{1,4},{1,5},{2,1},{2,2},{2,3},{2,4},{2,5},{3,1},{3,2},{3,3},{3,4},{3,5},{4,1},{4,2},{4,3},{4,4},{4,5},{5,1},{5,2},{5,3},{5,4},{5,5}};
    std::vector<std::pair<std::pair<int,int> ,int>>ans;
    auto ok = [&](int i, int j) -> bool
    {
        if(hiden1[i][j]==0)
        {
            if(maze1[i][j]!=0)
            {
                return false;
            }
        }
        else if(hiden1[i][j]==2)
        {
            if(maze1[i][j]==1||maze1[i][j]==3||maze1[i][j]==4)
            {
                return false;
            }
        }
        else if(hiden1[i][j]==1)
        {
            if(maze1[i][j]==0||maze1[i][j]==2||maze1[i][j]==4)
            {
                return false;
            }
        }
        else if(hiden1[i][j]==4)
        {
            if(maze1[i][j]!=4)
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
        maze1[x1][y1] += c;
        maze1[x1][y2+1] -=c;
        maze1[x2+1][y1] -=c;
        maze1[x2+1][y2+1] += c;
    };

    auto add_ = [&]() -> bool
    {
        int cnt=0;
        for (int i=1; i<=6; i++)
        {
            for (int j=1; j<=6; j++)
            {
                maze1[i][j] += maze1[i-1][j]+maze1[i][j-1]-maze1[i-1][j-1];
                if(maze1[i][j]>4)
                {
                    return false;
                }
            }
        }
        for (int i=1; i<=6; i++)
        {
            for (int j=1; j<=6; j++)
            {
                if(hiden1[i][j]==1&&(maze1[i][j]==0||maze1[i][j]==2))
                {
                    cnt++;
                    // cout<<'x'<<'\n';
                    // cout<<i<<' '<<j<<'\n';
                    maze1[i][j]++;
                }
                if( hiden1[i][j]==2&&maze1[i][j]==1)
                {
                    cnt++;
                    //     cout<<'y'<<'\n';
                    // cout<<i<<' '<<j<<'\n';
                    maze1[i][j]++;
                }
                if(hiden1[i][j]==4)
                {
                    cnt+=4-maze1[i][j];
                    maze1[i][j]=4;
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
        hiden1[x][y] = hiddenMap[i];
        qDebug()<<hiden1[x][y];
    }

    for(int i=1;i<=9;i++)
    {
        for(int j=1;j<=16;j++)
        {
            for(int k=1;k<=16;k++)
            {
                for(int h=1;h<=25;h++)
                {
                    for(int f=1;f<=25;f++)
                    {
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
                        //  for (int i=1; i<=6; i++)
                        //     {
                        //         for (int j=1; j<=6; j++)
                        //         {
                        //             maze[i][j] += maze[i-1][j]+maze[i][j-1]-maze[i-1][j-1];
                        //             cout<<maze[i][j]<<' ';
                        //         }
                        //     }
                        //     cout<<'\n';
                        for(int l=0;l<=35;l++)
                        {
                            int x=l/6+1;
                            int y=l%6+1;
                            maze1[x][y]=0;
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

//计算难度，计算拼图的密集程度
double calculateDensity(const std::vector<int>& map) {
    // 计算地图上拼图的数量
    int totalPuzzleCount = 0;
    for (int val : map) {
        if (val > 0)
        {
            totalPuzzleCount++;
        }
    }
    // 计算密集程度，即拼图的数量与总格子数的比例
    double density = static_cast<double>(totalPuzzleCount) / map.size();

    return density;
}


//计算难度
double calculateDifficulty(const std::vector<int>& map)
{
    std::vector<double> difficultyValues =
    {
        24.1, 19.5, 22.1, 20.1, 20.1, 22.1, 23.1, 19.1, 21.8, 20.5,
        21.1, 21.1, 22.5, 20.8, 23.1, 22.1, 23.1, 19.8, 20.5, 21.1,
        20.1, 20.1, 18.8, 20.8, 21.1, 20.1, 24.1, 24.1, 21.1, 22.1,
        23.1, 21.1, 19.1, 21.1, 22.1, 20.8, 22.1, 23.1, 20.5, 22.1,
        19.5, 19.2, 16.9, 18.2, 19.2, 21.1, 19.8, 17.5, 19.8, 21.8,
        21.1, 24.1, 21.1, 20.1, 24.1, 22.1, 22.1, 22.1, 21.1
    };
    // 计算密集程度
    double density = calculateDensity(map);
    // 计算每种类型拼图的数量
    int count_2x2 = 0, count_3x3 = 0, count_4x4 = 0;
    for (int val : map)
    {
        switch (val)
        {
        case 1:
            count_3x3++;
        case 2:
            count_2x2++;
            break;
        case 3:
            count_3x3++;
            break;
        case 4:
            count_4x4++;
            break;
        default:
            break;
        }
    }

    // 根据拼图数量和密集程度计算难度系数
    double difficulty = 0.0;
    // 越密集的拼图，增加的难度越大
    difficulty += count_2x2 * 0.4 * density; // 2x2拼图的权重乘以密集程度
    difficulty += count_3x3 * 0.5 * density; // 3x3拼图的权重乘以密集程度
    difficulty += count_4x4 * 0.1 * density; // 4x4拼图的权重乘以密集程度
    // 找到最大值和最小值
    double minDifficulty = *std::min_element(difficultyValues.begin(), difficultyValues.end());
    double maxDifficulty = *std::max_element(difficultyValues.begin(), difficultyValues.end());
    difficulty = (difficulty - minDifficulty) / (maxDifficulty - minDifficulty);
    return difficulty;
}

void FreeModePage::initMap()
{
    std::srand(std::time(nullptr)); // 使用当前时间作为随机数种子
    std::random_device rd;  //如果可用的话，从一个随机数发生器上获得一个真正的随机数
    std::mt19937 gen(rd()); //gen是一个使用rd()作种子初始化的标准梅森旋转算法的随机数发生器
    std::uniform_int_distribution<> distrib(0, 10000000);
    const int mapSize = 6;
    std::vector<int> map(mapSize * mapSize, 0); // 一维地图向量
    std::vector<Puzzle> puzzles =
        {
            Puzzle(PT_1x1, 1, 0, 0),
            Puzzle(PT_1x1, 1, 0, 0),
            Puzzle(PT_2x2, 1, 0, 0),
            Puzzle(PT_2x2, 1, 0, 0),
            Puzzle(PT_3x3, 1, 0, 0),
            Puzzle(PT_3x3, 1, 0, 0),
            Puzzle(PT_4x4, 1, 0, 0)
        };
    while(1)
    {
        for (int i = 0; i <= 35; i++)
        {
            map[i] = 0;
        }
        int placed = 0;
        for (Puzzle& puzzle : puzzles)
        {
            puzzle.x = (distrib(gen)+rand()) % (6 - puzzle.type);
            puzzle.y = (distrib(gen)+rand()) % (6 - puzzle.type);
            qDebug()<<puzzle.type<<'\n';
            //得到拼图的行与列
            if (puzzle.type == 0) // 修正：当使用1x1按钮时，直接更新单个格子的文本
            {
                map[puzzle.x * 6 + puzzle.y]++;
                continue;
            }
            // 检查是否越界、与其他拼图重叠以及是否有超过4的格子
            if (puzzle.x + puzzle.type+1 > 6 || puzzle.y + puzzle.type+1 > 6)
            {
                placed = 1;
                for (int i = 0; i <= 35; i++)
                {
                    map[i] = 0;
                }
                break;
            }
            for (int dx = puzzle.x; dx < puzzle.type+1 + puzzle.x; ++dx)
            {
                for (int dy = puzzle.y; dy < puzzle.type+1 + puzzle.y; ++dy)
                {
                    int index = dx * 6 +dy;
                    map[index]++;
                }
            }
            // 更新地图
        }
        if (placed)
        {
            continue;
        }
        int flag = 0;
        int cnt=0;
        for (int i = 0; i <= 35; i++)
        {
            if(map[i]==4)
            {
                cnt++;
            }
            if (map[i] > 4)
            {
                flag = 1;
                break;
            }
        }
        if (flag||cnt<1)
        {
            for (int i = 0; i <= 35; i++)
            {
                map[i] = 0;
            }
            continue;
        }
        else
        {
            break;
        }
    }

    for(int i=0;i<=35;i++)
    {
        if(map[i]==0)
        {
            map[i]=2;
        }
        if(map[i]==3)
        {
            map[i]=1;
        }
    }
    double difficulty = calculateDifficulty(map);
    // 写入C盘文件
    QFile file("./xxx.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (int val : map)
        {
            out << val << '\n';
        }
        file.close();
    }
    else
    {
        qWarning() << "Failed to open file for writing";
    }
    // 写入C盘文件
    QFile _file("./tiku.txt");
    if (_file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&_file);
       // 写入地图
        for (int i = 0; i < map.size(); ++i)
        {
            out << map[i];
            if ((i + 1) % mapSize == 0) {
                out << '\n'; // 每行结尾添加换行符
            } else {
                out << ' '; // 格子之间用空格隔开
            }
        }
        //写入难度系数
        out << difficulty << '\n';
        _file.close();
    }
    else
    {
        qWarning() << "Failed to open file for writing";
    }

    update(QStringLiteral("./xxx.txt"));
}


