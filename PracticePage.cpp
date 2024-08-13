#include "PracticePage.h"
#include"LevelOnepa.h"
#include<QInputDialog>
#include<QFile>
#include<QInputDialog>
#include "CustomInputDialog.h"
PracticePage::PracticePage(QWidget *parent)
    : QWidget(parent),
    levelOnePa(new LevelOnepa(this)),
    stackedWidget(new QStackedWidget(this))
{
    // 设置窗口大小
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

    vLayout = new QVBoxLayout(this);
    returnButton = new QPushButton("返回", this);
    returnButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");

    // 将返回按钮添加到布局
    vLayout->addWidget(returnButton, 0, Qt::AlignLeft | Qt::AlignTop);

    //水平布局
    QVBoxLayout *hlayout = new QVBoxLayout();
    // 修改按钮间距
    hlayout->setSpacing(20);
    // 创建并设置关卡按钮
    QString buttonStyle = "QPushButton {"
                          "background-color: transparent; border: none; border-radius: 0px; "
                          "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                          "}"
                          "QPushButton:hover {"
                          "color: lightgray;"  // 悬停时的颜色变化
                          "}";

    //关卡一，关卡二，关卡三创建
    levelOneButton = new QPushButton("关卡 1", this);
    levelOneButton->setStyleSheet(buttonStyle);
    levelOneButton->setFixedSize(250, 100);
    hlayout->addWidget(levelOneButton);

    levelTwoButton = new QPushButton("关卡 2", this);
    levelTwoButton->setStyleSheet(buttonStyle);
    levelTwoButton->setFixedSize(250, 100);
    hlayout->addWidget(levelTwoButton);

    levelThreeButton = new QPushButton("关卡 3", this);
    levelThreeButton->setStyleSheet(buttonStyle);
    levelThreeButton->setFixedSize(250, 100);
    hlayout->addWidget(levelThreeButton);

    QPushButton *zidingyiButton = new QPushButton("自定义难度", this);
    zidingyiButton->setStyleSheet(buttonStyle);
    zidingyiButton->setFixedSize(250, 100);
    hlayout->addWidget(zidingyiButton);


    // 可选：在关卡按钮下方添加一定间距
    hlayout->setContentsMargins(500, 340, 0, 0);

    // 创建垂直布局来容纳按钮和垂直间距
    QVBoxLayout *vButtonLayout = new QVBoxLayout();
    vButtonLayout->addLayout(hlayout);

    // 添加垂直间距
    QSpacerItem *verticalSpacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vButtonLayout->addSpacerItem(verticalSpacer);
    vLayout->addLayout(vButtonLayout);

    //主页面
    QWidget *mainPage = new QWidget(this);
    mainPage->setLayout(vLayout);
    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(levelOnePa);

    //连接槽函数，复用+返回
    connect(levelOneButton, &QPushButton::clicked, this, &PracticePage::readlevelone);
    connect(levelTwoButton, &QPushButton::clicked, this, &PracticePage::readleveltwo);
    connect(levelThreeButton, &QPushButton::clicked, this, &PracticePage::readlevelthree);
    connect(returnButton,&QPushButton::clicked,this,&PracticePage::returnToMainPage);
    connect(levelOneButton, &QPushButton::clicked, this, &PracticePage::showLevelOnePa);
    connect(levelTwoButton, &QPushButton::clicked, this, &PracticePage::showLevelOnePa);
    connect(levelThreeButton, &QPushButton::clicked, this, &PracticePage::showLevelOnePa);
    connect(levelOnePa,&LevelOnepa::returnToPracticePage,this,&PracticePage::on_ReturnToPracticePage);
    connect(zidingyiButton, &QPushButton::clicked, this, &PracticePage::onCustomDifficultyButtonClicked);
   // connect(zidingyiButton, &QPushButton::clicked, this, &PracticePage::showLevelOnePa);
}

PracticePage::~PracticePage()
{
}

void PracticePage::on_ReturnButton_clicked()
{
    // 发出返回主页面信号
    emit returnToMainPage();
}

//复用地图的三个函数
void PracticePage::readlevelone()
{
    levelOnePa->_nowstate="关卡一";
    levelOnePa->on_ResetButton_clicked();
    levelOnePa->update(":/new/images/qqzl.txt");
}

void PracticePage::readleveltwo()
{
    levelOnePa->_nowstate="关卡二";
    levelOnePa->on_ResetButton_clicked();
    levelOnePa->update(":/new/images/qqzlx.txt");
}

void PracticePage::readlevelthree()
{
    levelOnePa->_nowstate="关卡三";
    levelOnePa->on_ResetButton_clicked();
    levelOnePa->update(":/new/images/qqzlt.txt");
}

// 更新信息
void PracticePage::updateinformation()
{
    levelOnePa->_username=this->_username;
    levelOnePa->_password=this->_password;
    levelOnePa->imagePath=this->imagePath;
}

//显示关卡一，关卡二，关卡三，期间主要复用
void PracticePage::showLevelOnePa()
{
    stackedWidget->setCurrentIndex(1);
}

//返回练习界面
void PracticePage::on_ReturnToPracticePage()
{
    stackedWidget->setCurrentIndex(0);
}


void PracticePage::onCustomDifficultyButtonClicked()
{
    bool ok=1;
    double upperBound=-1;
    double lowerBound=-1;
    CustomInputDialog dialog(this);
    lowerBound = dialog.getDoubleValue(tr("自定义难度"), tr("请输入下限难度系数:"), 0.0, 0.0, 1.0);
    upperBound = dialog.getDoubleValue(tr("自定义难度"), tr("请输入上限难度系数:"), 1.0, 0.0, 1.0);

    qDebug()<<lowerBound<<upperBound;
    if (ok&&upperBound>=0&&lowerBound>=0)
    {
        // 打开文件并读取地图和难度系数
        QFile file("./tiku.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            // 在所有地图中选择匹配的地图
            int index = -1;
            // 逐行读取地图和对应的难度系数
            while (!in.atEnd())
            {
                QVector<int> map;
                for (int i = 0; i < 6; ++i)
                {
                    QString line = in.readLine();
                    QStringList tokens = line.split(" ");
                    for (const QString& token : tokens)
                    {
                        map.append(token.toInt());
                    }
                }
                double mapDifficulty = in.readLine().toDouble();
                if(mapDifficulty>=lowerBound&&mapDifficulty<=upperBound)
                {
                    QFile file("./xx.txt");
                    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                    {
                        QTextStream out(&file);
                        for (int val : map)
                        {
                            out << val << '\n';
                        }
                        file.close();
                        levelOnePa->_nowstate="自输入模式";//改成这样提交时候就不会影响到排行榜
                        levelOnePa->update("./xx.txt");
                        showLevelOnePa();
                        break;
                    }
                    else
                    {
                        qWarning() << "Failed to open file for writing";
                    }
                }
            }

            file.close();
            }
    }
}

