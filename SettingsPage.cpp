#pragma execution_character_set("utf-8") // 设置执行字符集为 UTF-8
#include "SettingsPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QScrollArea>
#include <QSpacerItem>
#include <QLabel>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

SettingsPage::SettingsPage(QWidget *parent) : QWidget(parent)
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

    // 创建返回按钮
    QPushButton *returnButton = new QPushButton("返回", this);
    returnButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 29px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");
    returnButton->setFixedSize(100, 50);

    // 设置图像垂直布局
    QVBoxLayout *imageLayout = new QVBoxLayout;

    // 创建一个水平布局，然后添加图片和相应的文字
    QHBoxLayout *row1 = new QHBoxLayout;
    QLabel *imageLabel1 = new QLabel(this);
    imageLabel1->setPixmap(QPixmap(":/new/images/px1.png"));
    imageLabel1->setScaledContents(true); // 启用自适应大小
    imageLabel1->setMaximumSize(70, 70); // 设置最大大小
    row1->addWidget(imageLabel1);

    QLabel *noTreeLabel1 = new QLabel("<b>代表该方格没有树</b>", this); // 加粗字体
    noTreeLabel1->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");
    noTreeLabel1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    row1->addWidget(noTreeLabel1);
    imageLayout->addLayout(row1);

    QHBoxLayout *row2 = new QHBoxLayout;
    QLabel *imageLabel2 = new QLabel(this);
    imageLabel2->setPixmap(QPixmap(":/new/images/px4.png"));
    imageLabel2->setScaledContents(true);
    imageLabel2->setMaximumSize(70, 70);
    row2->addWidget(imageLabel2);

    QLabel *noTreeLabel2 = new QLabel("<b>代表该方格有1或3颗树</b>", this); // 加粗字体
    noTreeLabel2->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");
    noTreeLabel2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    row2->addWidget(noTreeLabel2);
    imageLayout->addLayout(row2);

    QHBoxLayout *row3 = new QHBoxLayout;
    QLabel *imageLabel3 = new QLabel(this);
    imageLabel3->setPixmap(QPixmap(":/new/images/px2.png"));
    imageLabel3->setScaledContents(true);
    imageLabel3->setMaximumSize(70, 70);
    row3->addWidget(imageLabel3);

    QLabel *noTreeLabel3 = new QLabel("<b>代表该方格有2颗树或0颗树</b>", this); // 加粗字体
    noTreeLabel3->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");
    noTreeLabel3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    row3->addWidget(noTreeLabel3);
    imageLayout->addLayout(row3);

    QHBoxLayout *row4 = new QHBoxLayout;
    QLabel *imageLabel4 = new QLabel(this);
    imageLabel4->setPixmap(QPixmap(":/new/images/px3.png"));
    imageLabel4->setScaledContents(true);
    imageLabel4->setMaximumSize(70, 70);
    row4->addWidget(imageLabel4);

    QLabel *noTreeLabel4 = new QLabel("<b>代表该方格有4颗树</b>", this); // 加粗字体
    noTreeLabel4->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");
    noTreeLabel4->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    row4->addWidget(noTreeLabel4);
    imageLayout->addLayout(row4);

    // 最后一行字，作为帮助文档
    QLabel *noTreeLabel5 = new QLabel("<b>1.你需要通过点击按钮填充图形；</b><br><br> <b>2.点击之后，中间盘面格子右上角的数字代表该方格已经种树木的树量；</b><br><br><b>3.你需要使得每个格子的目标图案代表的数量跟所种的树木的数量匹配，即可通关；</b>", this); // 加粗字体，换行
    noTreeLabel5->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 24px; color: white;"); // 调整字体大小

    // 创建中心内容的布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 800, 160);
    mainLayout->setSpacing(20);

    // 添加返回按钮在左上角
    mainLayout->addWidget(returnButton, 0, Qt::AlignTop | Qt::AlignLeft);

    // 添加内容
    mainLayout->addLayout(imageLayout);

    // 添加具体说明
    mainLayout->addWidget(noTreeLabel5);

    // 设置主布局
    setLayout(mainLayout);

    // 连接返回按钮的点击事件
    connect(returnButton, &QPushButton::clicked, this, &SettingsPage::returnToMainPage);


}

SettingsPage::~SettingsPage()
{
}

void SettingsPage::on_ReturnButton_clicked()
{
    emit returnToMainPage();
}
