#include "CompetitionPage.h"
#include "HelpWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include<QGraphicsDropShadowEffect>
#include"chatroomwindow.h"
#include<QPropertyAnimation>

void CompetitionPage::showFreeModePage()
{
    stackedWidget->setCurrentIndex(1);
    freeModePage->_username=this->_username;
    freeModePage->_password=this->_password;
    freeModePage->imagePath=this->imagePath;
}
void CompetitionPage::showInputModePage()
{
    stackedWidget->setCurrentIndex(2);
}
void CompetitionPage::on_ReturnToCompetitionPage()
{
    stackedWidget->setCurrentIndex(0);
}

void CompetitionPage::showChatRoom()
{
    stackedWidget->setCurrentIndex(3);
    chatRoom->_username=this->_username;
    chatRoom->_password=this->_password;
    chatRoom->imagePath=this->imagePath;
}

CompetitionPage::CompetitionPage(QWidget *parent)
    : QWidget(parent),
    freeModePage(new FreeModePage(this)),
    inputModePage(new InputModePage(this)),
    stackedWidget(new QStackedWidget(this)),
    chatRoom(new ChatRoomWindow(this))
{
    //创建垂直布局
    setFixedSize(1800, 1200);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框对话框
    // 设置窗口位置，使其向下移动300像素
    move(100, 300);

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
    vLayout->addWidget(returnButton, 0, Qt::AlignLeft | Qt::AlignTop);

    //创建垂直布局
    QVBoxLayout *hlayout = new QVBoxLayout();
    hlayout->setSpacing(50);

    //设置按钮样式
    QString buttonStyle = "QPushButton {"
                          "background-color: transparent; border: none; border-radius: 0px; "
                          "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                          "}"
                          "QPushButton:hover {"
                          "color: lightgray;"  // 悬停时的颜色变化
                          "}";

    //自由模式按钮
    freeModeButton = new QPushButton("自由模式", this);
    freeModeButton->setStyleSheet(buttonStyle);
    freeModeButton->setFixedSize(250, 100);
    hlayout->addWidget(freeModeButton);

    //输入模式按钮
    inputModeButton = new QPushButton("输入模式", this);
    inputModeButton->setStyleSheet(buttonStyle);
    inputModeButton->setFixedSize(250, 100);
    hlayout->addWidget(inputModeButton);

    //聊天室按钮
    charButton=new QPushButton("聊天室",this);
    charButton->setStyleSheet(buttonStyle);
    charButton->setFixedSize(250, 100);
    hlayout->addWidget(charButton);

    hlayout->setContentsMargins(500, 340, 0, 0);

    // 创建垂直布局来容纳按钮和垂直间距
    QVBoxLayout *vButtonLayout = new QVBoxLayout();
    vButtonLayout->addLayout(hlayout);

    // 添加垂直间距
    QSpacerItem *verticalSpacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vButtonLayout->addSpacerItem(verticalSpacer);
    vLayout->addLayout(vButtonLayout);


    //创建主页面并添加其他页面
    QWidget *mainPage = new QWidget(this);
    mainPage->setLayout(vLayout);
    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(freeModePage);
    stackedWidget->addWidget(inputModePage);
    stackedWidget->addWidget(chatRoom);

    //连接槽函数
    connect(returnButton, &QPushButton::clicked, this, &CompetitionPage::on_ReturnButton_clicked);
    connect(freeModeButton, &QPushButton::clicked, this, &CompetitionPage::showFreeModePage);
    connect(inputModeButton, &QPushButton::clicked, this, &CompetitionPage::showInputModePage);
    connect(charButton,&QPushButton::clicked,this,&CompetitionPage::showChatRoom);
    connect(freeModePage,&FreeModePage::returnToCompetitionPage,this,&CompetitionPage::on_ReturnToCompetitionPage);
    connect(inputModePage,&InputModePage::returnToCompetitionPage,this,&CompetitionPage::on_ReturnToCompetitionPage);
    connect(chatRoom,&ChatRoomWindow::returnToCompetitionPage,this,&CompetitionPage::on_ReturnToCompetitionPage);
}

CompetitionPage::~CompetitionPage()
{
}

//返回主页面
void CompetitionPage::on_ReturnButton_clicked()
{
    emit returnToMainPage();
}

//展示帮助文档
void CompetitionPage::showHelpWindow()
{
    HelpWindow *helpWindow = new HelpWindow(this);
    helpWindow->setAttribute(Qt::WA_DeleteOnClose);
    helpWindow->show();
}

void  CompetitionPage::showCompetitionPage(){}

//更新用户名，用户，路径
void CompetitionPage::updateInformation()
{

}
