#include "MsgBox.h"
#include <QGraphicsOpacityEffect>

MsgBox::MsgBox(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("加入请求");
    setFixedSize(600, 300);  // 调整对话框大小
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框对话框

    // 设置背景颜色和透明度
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);

    // 设置消息文本
    messageLabel = new QLabel("客户端向您发起对战邀请？", this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");

    // 设置按钮
    yesButton = new QPushButton("接受", this);
    noButton = new QPushButton("拒绝", this);

    yesButton->setStyleSheet("QPushButton {"
                             "background-color: #FF0000; border: none; border-radius: 10px; "
                             "font-family: 'Microsoft YaHei'; font-size: 23px; color: white;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #CC0000;"
                             "}");
    yesButton->setFixedSize(150, 50);

    noButton->setStyleSheet("QPushButton {"
                            "background-color: #808080; border: none; border-radius: 10px; "
                            "font-family: 'Microsoft YaHei'; font-size: 23px; color: white;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #696969;"
                            "}");
    noButton->setFixedSize(150, 50);

    // 布局设置
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(messageLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(yesButton);
    buttonLayout->addSpacing(30);
    buttonLayout->addWidget(noButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(30);

    // 信号与槽连接
    connect(yesButton, &QPushButton::clicked, this, &MsgBox::onYesClicked);
    connect(noButton, &QPushButton::clicked, this, &MsgBox::onNoClicked);
}

MsgBox::~MsgBox()
{
}

void MsgBox::onYesClicked()
{
    accept();
}

void MsgBox::onNoClicked()
{
    reject();
}
