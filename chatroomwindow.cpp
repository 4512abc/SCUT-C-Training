#include "chatroomwindow.h"
#include <QPushButton>
#include <QVBoxLayout>

ChatRoomWindow::ChatRoomWindow(QWidget *parent)
    : QWidget(parent),
    clientWidget(nullptr),
    serverWidget(nullptr)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QPushButton *returnButton = new QPushButton("返回", this);
    returnButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");
    vLayout->addWidget(returnButton, 0, Qt::AlignLeft | Qt::AlignTop);

    //创建垂直布局来容纳按钮
    QVBoxLayout *hlayout = new QVBoxLayout();
    // 修改按钮间距
    hlayout->setSpacing(100);

    //设置按钮样式
    QString buttonStyle = "QPushButton {"
                          "background-color: transparent; border: none; border-radius: 0px; "
                          "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                          "}"
                          "QPushButton:hover {"
                          "color: lightgray;"  // 悬停时的颜色变化
                          "}";

    //添加客户端按钮
    QPushButton *connectServerButton = new QPushButton("客户端",this);
    connectServerButton->setStyleSheet(buttonStyle);
    connectServerButton->setFixedSize(250, 100);
    hlayout->addWidget(connectServerButton);

    //添加服务端按钮
    QPushButton *startServerButton = new QPushButton("服务端",this);
    startServerButton->setStyleSheet(buttonStyle);
    startServerButton->setFixedSize(250, 100);
    hlayout->addWidget(startServerButton);

    //在按钮下方添加一定的间距
    hlayout->setContentsMargins(500, 400, 0, 0);

    // 创建垂直布局来容纳按钮和垂直间距
    QVBoxLayout *vButtonLayout = new QVBoxLayout();
    vButtonLayout->addLayout(hlayout);

    //创建了一个垂直的空白间隔项，表示空白间隔保持最小尺寸，垂直分向上可以填充到任何额外的空间
    QSpacerItem *verticalSpacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vButtonLayout->addSpacerItem(verticalSpacer);
    vLayout->addLayout(vButtonLayout);

    //创建主页面的QWidget
    QWidget *mainPage = new QWidget(this);
    mainPage->setLayout(vLayout);

    //连接槽函数
    connect(connectServerButton, &QPushButton::clicked, this, &ChatRoomWindow::on_connectServerButton_clicked);
    connect(startServerButton, &QPushButton::clicked, this, &ChatRoomWindow::on_startServerButton_clicked);
    connect(returnButton, &QPushButton::clicked, this, &ChatRoomWindow::returnToCompetitionPage);
}
ChatRoomWindow::~ChatRoomWindow()
{
    if (clientWidget)
        delete clientWidget;
    if (serverWidget)
        delete serverWidget;
    //清理服务端和客户端的资源
}

// 处理客户端按钮点击事件的槽函数
void ChatRoomWindow::on_connectServerButton_clicked()
{
    if (!clientWidget)
    {
        clientWidget = new ClientWidget;
        //这个代码作用是获取当前的窗口标志，然后使用位或运算符指示窗口应该保持在其他窗口的上方
        clientWidget->setWindowFlags(clientWidget->windowFlags() | Qt::WindowStaysOnTopHint);
        clientWidget->show();
        clientWidget->_username=this->_username;
    }
    else
    {
        clientWidget->setWindowFlags(clientWidget->windowFlags() | Qt::WindowStaysOnTopHint);
        clientWidget->show();
    }
}

//创建服务端按钮点击事件逻辑的槽函数
void ChatRoomWindow::on_startServerButton_clicked()
{
    if (!serverWidget)
    {
        serverWidget = new ServerWidget;
        serverWidget->setWindowFlags(serverWidget->windowFlags() | Qt::WindowStaysOnTopHint);
        serverWidget->show();
        serverWidget->_username=this->_username;
    }
    else
    {
        serverWidget->setWindowFlags(serverWidget->windowFlags() | Qt::WindowStaysOnTopHint);
        serverWidget->show();
    }
}

//返回竞赛页面
void ChatRoomWindow::on_ReturnButton_clicked()
{
    emit returnToCompetitionPage();
}
