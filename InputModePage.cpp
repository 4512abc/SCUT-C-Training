#include "InputModePage.h"
#include "CompetitionPage.h"
#include"SuccessDialog.h"
#include"FailureDialog.h"
#include <QRandomGenerator>
#include<QPalette>
#include<QMessageBox>
#include<QFile>
#include<QInputDialog>
#include "CustomHintDialog.h"
#include "inputdialog.h"
#include "custommessagebox.h"
InputModePage::InputModePage(CompetitionPage* parent)
    : QWidget(parent)
{
    QString buttonStyle =(   "QPushButton {"
                           "background-color: transparent; border: 2px solid white; border-radius: 10px; "
                           "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                           "}"
                           "QPushButton:hover {"
                           "color: lightgray;"  // 悬停时的颜色变化
                           "}");
    QVBoxLayout *vLayout = new QVBoxLayout();
    // 初始化隐藏地图数据
    timerLabel = new QLabel("时间：0 秒", this);
    timerLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white; border: 2px solid black; padding: 5px;");
    // 初始化隐藏地图数据
    hiddenMap.resize(36);
    readHiddenMapFromFile(QStringLiteral(":/new/images/qqzl.txt"), hiddenMap);
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
    connect(returnButton, &QPushButton::clicked, this, &InputModePage::returnToCompetitionPage);

    resetButton = new QPushButton("重置", this);
    resetButton->setStyleSheet(buttonStyle);
    resetButton->setIcon(QIcon(":/new/images/cxks.png"));
    resetButton->setIconSize(QSize(41, 41));
    connect(resetButton, &QPushButton::clicked, this, &InputModePage::on_ResetButton_clicked);

    // 创建回退按钮
    undoButton = new QPushButton("撤销", this);
    undoButton->setStyleSheet(buttonStyle);
    undoButton->setIcon(QIcon(":/new/images/cx.png"));
    undoButton->setIconSize(QSize(41, 41));
    connect(undoButton, &QPushButton::clicked, this, &InputModePage::undoLastOperation);

    submitButton = new QPushButton("提交", this);
    submitButton->setStyleSheet(buttonStyle);
    submitButton->setIcon(QIcon(":/new/images/tj.png"));
    submitButton->setIconSize(QSize(41, 41));
    connect(submitButton, &QPushButton::clicked, this, &InputModePage::on_SubmitButton_clicked);

    // 创建开始按钮
    startButton = new QPushButton("开始", this);
    startButton->setStyleSheet(buttonStyle);
    startButton->setIcon(QIcon(":/new/images/ks.png"));
    startButton->setIconSize(QSize(41, 41));
    connect(startButton, &QPushButton::clicked, this, &InputModePage::startGame);


    editMapButton = new QPushButton("编辑地图", this);
    editMapButton->setStyleSheet(buttonStyle);
    editMapButton->setIcon(QIcon(":/new/images/zsr.png"));
    editMapButton->setIconSize(QSize(41,41));
    connect(editMapButton, &QPushButton::clicked, this, &InputModePage::openMapinputDialog);


    // 创建提示按钮
    QPushButton *hintButton = new QPushButton("提示", this);
    hintButton->setStyleSheet(buttonStyle);
    hintButton->setIcon(QIcon(":/new/images/tip.png"));
    hintButton->setIconSize(QSize(41, 41));
    connect(hintButton, &QPushButton::clicked, this, &InputModePage::showhint);
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
            textCell->setStyleSheet(" font-weight: bold; color: white; font-size: 20pt; text-align: center; vertical-align: middle");
            textCell->setText(QString::number(0));

            // 创建特殊符号图片标签
            QLabel *symbolCell = new QLabel(this);
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
            QWidget *cellWrapper = new QWidget(this);
            cellWrapper->setLayout(cellLayout);
            puzzleCells[row * puzzleSize + col] = textCell;
            puzzleGrid->addWidget(cellWrapper, row, col);
        }
    }
    // vLayout->addLayout(puzzleGrid);
    //  vLayout->addWidget(timerLabel); // 将计时器标签添加到垂直布局中
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
    //添加水平布局
    // 创建新的水平主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左侧垂直布局，包含返回按钮和网格大小选择按钮
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0,0,0,400);
    leftLayout->addWidget(returnButton);
    leftLayout->addSpacing(100);
    leftLayout->addWidget(timerLabel);  // 将计时器标签添加到左侧布局中
    leftLayout->addSpacing(100);
    leftLayout->addLayout(gridSizeLayout);
    // 设置左侧布局的左边距
    // 设置左边距为50像素

    // 中间布局，包含地图
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->addLayout(puzzleGrid); // 添加你的地图布局
    centerLayout->addWidget(timerLabel); // 添加计时器标签
    // 设置左侧布局的左边距
    centerLayout->setContentsMargins(125, 0, 0, 0); // 设置左边距为50像素

    // 右侧垂直布局，包含开始、撤销、重置和提交按钮
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(100,100,0,300);
    //  rightLayout->addWidget(timerLabel);
    rightLayout->addWidget(editMapButton);
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

    // 设置布局之间的间距和对齐方式
    mainLayout->addStretch(1);  // 添加一个可伸缩的空间，使得布局在垂直方向上居中
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addLayout(rightLayout);

    // 设置主布局
    this->setLayout(mainLayout);
    // mainLayout->addLayout(gridSizeLayout);
    // 添加左侧布局（vLayout）

    //  mainLayout->addLayout(vLayout);
    // 设置左侧布局的权重为 1，使其占据一半空间

    // 添加右侧布局（bottomButtonLayout）
    //  mainLayout->addLayout(bottomButtonLayout);
    // 设置右侧布局的权重为 1，使其占据另一半空间

    connect(timer, SIGNAL(timeout()), this, SLOT()); // SLOT填入一个槽函数
    //   timer->start(1000); // 1000毫秒, 等于 1 秒
    connect(timer, &QTimer::timeout, this, &InputModePage::updateTimer);

    // 设置计时器间隔为1秒
    timer->setInterval(1000); // 1000毫秒 = 1秒
     timer->stop(); // 启动计时器
    // 设置主布局为窗口布局
    this->setLayout(mainLayout);
}

void InputModePage::startGame()
{
    // 初始化计时器
    elapsedTimeSeconds = 0;
    timerLabel->setText("时间：0 秒");
    timer->start();

    // 初始化隐藏地图
    readHiddenMapFromFile(QStringLiteral(":/new/images/qqzl.txt"), hiddenMap);

    // 初始化所有格子数值为0
    for (int i = 0; i < 36; ++i) {
        puzzleCells[i]->setText("0");
    }

    // 清空所有按钮使用记录
    for (int i = 0; i < useCounts.size(); ++i) {
        useCounts[i] = 0;
    }

    // 清空撤销栈
    undoStack.clear();

    // 初始化其他相关状态变量
    isButtonClicked = false;
}



void InputModePage::on_ReturnButton_clicked()
{
    emit returnToCompetitionPage();
}


void InputModePage::on_PuzzleCell_clicked(QLabel *cell, int clickedRow, int clickedCol)
{
    if (!isButtonClicked)
    { // 检查标志变量状态
        return; // 若未点击按钮，则不处理格子点击事件，直接返回
    }

    int bottomRow = clickedRow + currentGridSize - 1;
    int rightCol = clickedCol + currentGridSize - 1;

    if (bottomRow >= puzzleSize || rightCol >= puzzleSize)
    {
        // 点击区域超出了拼图边界，此次操作无效
        CustomMessageBox messageBox(tr("您选择的区域超过了拼图边界！"));
        messageBox.exec();
        return ;
    }
    if (currentGridSize == 1) // 修正：当使用1x1按钮时，直接更新单个格子的文本
    {
        cell->setText(QString::number(cell->text().toInt() + 1));
    }
    else
    {
        for (int row = clickedRow; row < std::min(clickedRow + currentGridSize, 6); ++row) {
            for (int col = clickedCol; col < std::min(clickedCol + currentGridSize, 6); ++col) {

                QLabel *targetCell = puzzleCells[row * puzzleSize + col];
                targetCell->setText(QString::number(targetCell->text().toInt() + 1));
            }
        }
    }
    isButtonClicked=false;
    useCounts[currentGridSize]++;
    // 记录当前操作信息
    QString operationDescription = QStringLiteral("%1,%2,%3").arg(clickedRow).arg(clickedCol).arg(currentGridSize);
    undoStack.push(operationDescription);
}

void InputModePage::on_GridSizeButton_clicked(int size)
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

    if (useCounts[size] >= maxUseCounts[size ])
    { // 检查当前按钮的计数器是否已达最大使用次数
        return; // 若已达最大使用次数，则忽略此次点击事件
    }

    currentGridSize = size;


    isButtonClicked = true; // 设置标志变量为 true，表示已点击按钮


}

void InputModePage::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return; // 仅处理左键点击事件
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
    for (QObject *child : cellWrapper->children()) {
        label = qobject_cast<QLabel *>(child);

        if (label && label->underMouse()) {
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
    }
    else
    {
        qWarning("No QLabel under mouse at clicked position: row=%d, col=%d", clickedRow, clickedCol);
    }
}

bool InputModePage::comparePuzzleWithHiddenMap()
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


void InputModePage::showSuccessDialog()
{
    SuccessDialog dialog(this);
    dialog.setFixedWidth(600); // 将宽度从 300 像素增加到 400 像素
    dialog.setFixedHeight(650); // 将高度从 100 像素增加到 150 像素
    dialog.exec();
}

void InputModePage::showFailureDialog()
{
    FailureDialog dialog(this);
    dialog.setFixedWidth(600); // 将宽度从 300 像素增加到 400 像素
    dialog.setFixedHeight(650); // 将高度从 100 像素增加到 150 像素
    dialog.exec();
}

void InputModePage::undoLastOperation()
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
    for (int row = clickedRow; row < std::min(clickedRow + gridSize, puzzleSize); ++row) {
        for (int col = clickedCol; col < std::min(clickedCol + gridSize, puzzleSize); ++col) {
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

void InputModePage::on_ResetButton_clicked()
{
    // 重置所有格子数值为0
    for (int i = 0; i < 36; ++i) {
        puzzleCells[i]->setText("0");
    }

    // 清空所有按钮使用记录
    for (int i = 0; i < useCounts.size(); ++i) {
        useCounts[i] = 0;
    }
    // 清空撤销栈
    undoStack.clear();

    isButtonClicked = false;

    elapsedTimeSeconds = 0;
    timerLabel->setText("时间：0 秒");
    timer->stop(); // 重新启动计时器
}

// 根据隐藏地图值设置对应格子的特殊符号
void InputModePage::setSpecialSymbolForCell(QLabel *cell, int hiddenValue)
{
    // 加载特殊符号图片
    QString imagePath = symbolFilePaths.value(hiddenValue);
    QPixmap symbolPixmap(imagePath);
    QPixmap picture1 ;
    cell->setPixmap(symbolPixmap);
    cell->setScaledContents(true);//自适应大小
}

void InputModePage::readHiddenMapFromFile(const QString &filePath, std::vector<int> &hiddenMap)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << filePath;
        return;
    }
    QTextStream in(&file);
    for (int i = 0; i < 36; ++i) {
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

void InputModePage::updateTimer()
{
    elapsedTimeSeconds++;
    timerLabel->setText(QString("时间：%1 秒").arg(elapsedTimeSeconds));
}

void InputModePage::on_SubmitButton_clicked()
{
    if (!isGameFinished||!comparePuzzleWithHiddenMap())
    {
        CustomMessageBox messageBox("不能提交，虚名乃是浮云啊少年！", this);
        messageBox.exec();
        return;
    }
}


void InputModePage::update(const QString &filePath)
{
    readHiddenMapFromFile(filePath,hiddenMap);
    for (int row = 0; row < puzzleSize; ++row)
    {
        for (int col = 0; col < puzzleSize; ++col)
        {
            QLabel *textCell = new QLabel(this);
            textCell->setFixedSize(70, 70);
            textCell->setStyleSheet(" font-weight: bold; color: white; font-size: 20pt; text-align: center; vertical-align: middle");
            textCell->setText(QString::number(0));

            // 创建特殊符号图片标签
            QLabel *symbolCell = new QLabel(this);
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
            QWidget *cellWrapper = new QWidget(this);
            cellWrapper->setLayout(cellLayout);
            puzzleCells[row * puzzleSize + col] = textCell;
            puzzleGrid->addWidget(cellWrapper, row, col);
        }
    }
}

void InputModePage::upset()
{
    hiddenMap.resize(36);
}

//比较自由模式唯一的区别，就是出现36个对话框供给输入。
void InputModePage::openMapinputDialog()
{
    bool ok;
std::vector<int> newHiddenMap(36);

for (int i = 0; i < 36; ++i)
{
    InputDialog inputDialog(this);
    inputDialog.resize(600,400);
    inputDialog.setWindowTitle(tr("编辑地图"));
    inputDialog.setLabelText(tr("请输入第%1个数字：").arg(i + 1));

    QString text = inputDialog.getTextValue();
    if (text.isEmpty())
    {
        return;
    }

    // 转换用户输入的文本为整数
    bool conversionOk;
    int value = text.toInt(&conversionOk);
    if (!conversionOk || value < 0 || value > 4)
    {
        CustomMessageBox messageBox("请输入一个有效的数字（0-4");
        messageBox.exec();
        return;
    }

    // 将值存入数组
    newHiddenMap[i] = value;
}

    // 更新hiddenMap
    hiddenMap = newHiddenMap;

    // 更新拼图格子的特殊符号
    updatePuzzleGridSymbols();

}

void InputModePage:: updatePuzzleGridSymbols()
{
    for (int row = 0; row < puzzleSize; ++row) {
        for (int col = 0; col < puzzleSize; ++col) {

            // 获取当前格子的特殊符号标签
            QWidget *cellWrapper = puzzleGrid->itemAt(row * puzzleSize + col)->widget();
            QLabel *symbolCell = nullptr;
            for (QObject *child : cellWrapper->children()) {
                symbolCell = qobject_cast<QLabel *>(child);
                if (symbolCell)
                {
                    break;
                }
            }

            if (symbolCell)
            {
                setSpecialSymbolForCell(symbolCell,  hiddenMap[row * puzzleSize + col]);
            }
        }
    }
}


void InputModePage::showhint()
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
int maze[10][10];
int hiden[10][10];
//这下就判断好了
//判断地图有没有解的问题，并给予用户提示
std::vector<std::pair<std::pair<int,int> ,int>> InputModePage::getans()
{

    std::pair<int,int>a[30]={{0,0},{1,1},{1,2},{1,3},{2,1},{2,2},{2,3},{3,1},{3,2},{3,3}};
    std::pair<int,int>b[30]={{0,0},{1,1},{1,2},{1,3},{1,4},{2,1},{2,2},{2,3},{2,4},{3,1},{3,2},{3,3},{3,4},{4,1},{4,2},{4,3},{4,4}};
    std::pair<int,int>c[30]={{0,0},{1,1},{1,2},{1,3},{1,4},{1,5},{2,1},{2,2},{2,3},{2,4},{2,5},{3,1},{3,2},{3,3},{3,4},{3,5},{4,1},{4,2},{4,3},{4,4},{4,5},{5,1},{5,2},{5,3},{5,4},{5,5}};
    std::vector<std::pair<std::pair<int,int> ,int>>ans;
    auto ok = [&](int i, int j) -> bool
    {
        if(hiden[i][j]==0)
        {
            if(maze[i][j]!=0)
            {
                return false;
            }
        }
        else if(hiden[i][j]==2)
        {
            if(maze[i][j]==1||maze[i][j]==3||maze[i][j]==4)
            {
                return false;
            }
        }
        else if(hiden[i][j]==1)
        {
            if(maze[i][j]==0||maze[i][j]==2||maze[i][j]==4)
            {
                return false;
            }
        }
        else if(hiden[i][j]==4)
        {
            if(maze[i][j]!=4)
            {
                return false;
            }
        }
        return true;
    };

    auto add = [&](int x1, int y1, int x2, int y2, int c)
    {
        maze[x1][y1] += c;
        maze[x1][y2+1] -=c;
        maze[x2+1][y1] -=c;
        maze[x2+1][y2+1] += c;
    };

    auto add_ = [&]() -> bool
    {
        int cnt=0;
        for (int i=1; i<=6; i++)
        {
            for (int j=1; j<=6; j++)
            {
                maze[i][j] += maze[i-1][j]+maze[i][j-1]-maze[i-1][j-1];
                if(maze[i][j]>4)
                {
                    return false;
                }
            }
        }
        for (int i=1; i<=6; i++)
        {
            for (int j=1; j<=6; j++)
            {
                if(hiden[i][j]==1&&(maze[i][j]==0||maze[i][j]==2))
                {
                    cnt++;
                    maze[i][j]++;
                }
                if( hiden[i][j]==2&&maze[i][j]==1)
                {
                    cnt++;
                    maze[i][j]++;
                }
                if(hiden[i][j]==4)
                {
                    cnt+=4-maze[i][j];
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
        hiden[x][y] = hiddenMap[i];
        qDebug()<<hiden[x][y];
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
                            maze[x][y]=0;
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



