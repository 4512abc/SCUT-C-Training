#include "HelpWindow.h"

HelpWindow::HelpWindow(QWidget *parent)
    : QWidget(parent)
{
    // 创建垂直布局管理器并设置边距
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);

    // 创建文本标签并设置文本内容和对齐方式，添加到布局中
    textLabel = new QLabel("欢迎来到比赛页面，\n在这里你可以自由的开始游戏，\n地图会是随机的，\n或者你可以输入地图，\n玩得愉快！");
    textLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(textLabel);

    // 创建关闭按钮并添加到布局中，并设置对齐方式
    closeButton = new QPushButton("关闭");
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    // 连接关闭按钮的点击事件到槽函数 close()
    connect(closeButton, &QPushButton::clicked, this, &HelpWindow::close);
}

HelpWindow::~HelpWindow()
{
    // 清理资源，如果有需要的话
}
