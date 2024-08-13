#include "PlayerDataWindow.h"
#include <QFile>
#include <QTextStream>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QDebug>
#include <QScrollArea> // 添加滚动区域的头文件

PlayerDataWindow::PlayerDataWindow(QWidget *parent) : QWidget(parent)
{
    resize(800, 800);
    setWindowTitle("玩家数据");

    // 标题
    titleLabel = new QLabel("玩家数据", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Microsoft YaHei", 24, QFont::Bold));

    // 用户信息
    userInfoLabel = new QLabel(this);
    userInfoLabel->setAlignment(Qt::AlignCenter);
    userInfoLabel->setFont(QFont("Microsoft YaHei", 16));

    // 头像
    avatarLabel = new QLabel(this);
    avatarLabel->setAlignment(Qt::AlignCenter);

    // 主布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(userInfoLabel);
    mainLayout->addWidget(avatarLabel);
    mainLayout->setAlignment(Qt::AlignCenter);

    // 添加关闭按钮
    QPushButton *closeButton = new QPushButton("关闭", this);
    closeButton->setStyleSheet("QPushButton {"
                               "background-color: #FF0000; border: none; border-radius: 10px; "
                               "font-family: 'Microsoft YaHei'; font-size: 25px; color: white;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #CC0000;"
                               "}");
    closeButton->setFixedSize(150, 50);
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);

    // 游戏数据布局放入滚动区域
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollContent = new QWidget();
    gameDataLayout = new QVBoxLayout(scrollContent);
    gameDataLayout->setAlignment(Qt::AlignTop); // 设置游戏数据顶部对齐
    scrollContent->setLayout(gameDataLayout);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true); // 设置滚动区域自适应内容大小
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 禁用水平滚动条
    mainLayout->addWidget(scrollArea);

    mainLayout->addWidget(closeButton);
    mainLayout->setSpacing(20); // 设置主布局的间距

    setLayout(mainLayout);

    // 设置窗口背景色和透明度
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);
}

void PlayerDataWindow::setPlayerInfo(const QString &username, const QString &avatarPath)
{
    avatarLabel->setPixmap(QPixmap(avatarPath).scaled(150, 150, Qt::KeepAspectRatio));
    userInfoLabel->setText("用户名: " + username);
}

void PlayerDataWindow::readGameData(const QString &username)
{
    clearGameData(); // 清空之前的游戏数据显示

    QFile file("./www.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件" << '\n';
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(" ");
        if (parts.size() >= 4 && parts[1] == username)
        {
            QString level = parts[0];
            QString seconds = parts[2];
            QString displayText =   level + ": 秒数: " + seconds+"秒";

            QLabel *dataLabel = new QLabel(displayText, this);
            dataLabel->setFont(QFont("Microsoft YaHei", 14));
            gameDataLayout->addWidget(dataLabel);
            gameDataLabels.append(dataLabel);
        }
    }

    file.close();
}

void PlayerDataWindow::clearGameData()
{
    // 清空之前的游戏数据显示
    for (auto label : gameDataLabels)
    {
        gameDataLayout->removeWidget(label);
        delete label;
    }
    gameDataLabels.clear();
}

void PlayerDataWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void PlayerDataWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}
