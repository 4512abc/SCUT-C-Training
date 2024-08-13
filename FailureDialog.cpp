#include "FailureDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>
#include <QDate>

FailureDialog::FailureDialog(QWidget *parent) : QDialog(parent)
{
    // 设置窗口大小
    setFixedSize(600, 300);
    setWindowTitle("失败");
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框对话框

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

    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 添加图片标签
    QLabel *imageLabel = new QLabel(this);
    QPixmap image(":/new/images/stlgx.png"); // 替换为你的图片路径
    if (!image.isNull())
    {
        imageLabel->setPixmap(image.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    mainLayout->addWidget(imageLabel, 0, Qt::AlignCenter);

    // 添加消息标签
    QLabel *messageLabel = new QLabel("很遗憾，你失败了。", this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setFont(QFont("Microsoft YaHei", 24, QFont::Bold)); // 字体更大
    messageLabel->setStyleSheet("QLabel { color: white; }");
    mainLayout->addWidget(messageLabel, 0, Qt::AlignCenter);

    // 随机生成鼓励的话
    QStringList encouragement =
        {
            "凉の鼓励：\n不要灰心，每次尝试都是一次学习\n请继续尝试吧",
            "凉の鼓励：\n失败只是成功之路上的一个里程碑\n请继续尝试吧",
            "凉の鼓励：\n勇气是你前进的唯一动力，继续加油\n请继续尝试吧",
            "凉の鼓励：\n只要你不放弃，一切都是可能的\n请继续尝试吧",
        };
    QString randomEncouragement = encouragement[QRandomGenerator::global()->bounded(encouragement.size())];

    // 添加鼓励标签
    QLabel *encouragementLabel = new QLabel(randomEncouragement, this);
    encouragementLabel->setAlignment(Qt::AlignCenter);
    encouragementLabel->setFont(QFont("Microsoft YaHei", 14));
    encouragementLabel->setStyleSheet("QLabel { color: white; }");
    mainLayout->addWidget(encouragementLabel, 0, Qt::AlignCenter);

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
    mainLayout->addLayout(buttonLayout);

    // 设置布局
    setLayout(mainLayout);

    // 设置窗口淡入效果，一个小小的动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    //完全透明效果
    animation->setEndValue(1);
    //完全不透明效果
    animation->start();

    // 连接信号和槽
    connect(closeButton, &QPushButton::clicked, this, &FailureDialog::close);
}

QSize FailureDialog::sizeHint() const
{
    return QSize(600, 300);
}
