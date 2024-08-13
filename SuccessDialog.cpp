#pragma execution_character_set("utf-8") // 设置执行字符集为 UTF-8
#include "SuccessDialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>
#include <QDebug>

SuccessDialog::SuccessDialog(QWidget *parent) : QDialog(parent)
{
    // 设置对话框大小和标题
    setFixedSize(600, 300); // 调整对话框大小
    setWindowTitle("成功");
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

    // 添加阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadowEffect);

    // 设置主布局，包含一个侧边栏布局和一个内容布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 创建侧边栏布局
    QVBoxLayout *sidebarLayout = new QVBoxLayout();

    // 添加图片标签
    QLabel *imageLabel = new QLabel(this);
    QPixmap image(":/new/images/libai.png"); // 替换为你的图片路径
    if (!image.isNull())
    {
        imageLabel->setPixmap(image.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        sidebarLayout->addWidget(imageLabel, 0, Qt::AlignCenter);
    }

    // 将侧边栏布局添加到主布局
    mainLayout->addLayout(sidebarLayout);

    // 创建主内容布局
    QVBoxLayout *contentLayout = new QVBoxLayout();

    // 添加成功信息标签
    QLabel *messageLabel = new QLabel("恭喜！你成功了！", this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setFont(QFont("Microsoft YaHei", 24, QFont::Bold));
    messageLabel->setStyleSheet("QLabel { color: white; }");
    contentLayout->addWidget(messageLabel, 0, Qt::AlignCenter);

    // 随机生成鼓励的话，提高用户体验感
    QStringList encouragement =
        {
            "诗仙的勉励\n春风得意马蹄疾\n一日看尽长安花",
            "诗仙的勉励\n我见青山多妩媚\n料青山见我应如是",
            "诗仙的勉励\n仰天大笑出门去\n我辈岂是蓬蒿人",
            "诗仙的勉励\n天生我材必有用\n千金散尽还复来",
            "诗仙的勉励\n功名万里外\n心事一杯中",
            "诗仙的勉励\n君子藏器于身\n待时而动",
            "诗仙的勉励\n男儿不展风云志\n空负天生八尺躯",
            "诗仙的勉励\n不要人夸好颜色\n只留清气满乾坤"
        };
    QString randomEncouragement = encouragement[QRandomGenerator::global()->bounded(encouragement.size())];
    QLabel *encouragementLabel = new QLabel(randomEncouragement, this);
    encouragementLabel->setAlignment(Qt::AlignCenter);
    encouragementLabel->setStyleSheet("QLabel { color: white; font-size: 16px; }");
    contentLayout->addWidget(encouragementLabel, 0, Qt::AlignCenter);

    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *closeButton = new QPushButton("关闭", this);
    closeButton->setStyleSheet("QPushButton {"
                               "background-color: #FF0000; border: none; border-radius: 10px; "
                               "font-family: 'Microsoft YaHei'; font-size: 18px; color: white;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #CC0000;"
                               "}");
    closeButton->setFixedSize(100, 40);


    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addSpacing(30);

    // 将按钮布局添加到主内容布局
    contentLayout->addLayout(buttonLayout);

    // 将主内容布局添加到主布局
    mainLayout->addLayout(contentLayout);

    // 设置布局
    setLayout(mainLayout);

    // 添加动画效果：淡入
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    // 连接信号和槽
    connect(closeButton, &QPushButton::clicked, this, &SuccessDialog::close);
}
