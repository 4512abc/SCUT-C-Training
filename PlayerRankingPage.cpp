#include "PlayerRankingPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include<QScrollArea>
#include<QFileInfo>
#include<QGraphicsDropShadowEffect>
PlayerRankingPage::PlayerRankingPage(QWidget *parent)
    : QWidget(parent)
{
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadowEffect);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);

    QFile file("./www.txt");
    QFileInfo fileInfo(file);

    //如果为空就写进去作为内测数据
    if (fileInfo.size() == 0)
    {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << "关卡一 放课后TeaTime 0秒 :/new/images/background4.png\n";
            out << "关卡二 放课后TeaTime 0秒 :/new/images/background4.png\n";
            out << "关卡三 放课后TeaTime 1秒 :/new/images/background4.png\n";
            file.close();
        }
    }
    else{}
    QString buttonStyle =         "QPushButton {"
                          "background-color: transparent; border: 2px solid white; border-radius: 10px; "
                          "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                          "}"
                          "QPushButton:hover {"
                          "color: lightgray;"  // 悬停时的颜色变化
                          "}";
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 0);

    // 创建并添加返回按钮
    returnButton = new QPushButton("返回", this);
    returnButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");
    mainLayout->addWidget(returnButton, 0, Qt::AlignTop | Qt::AlignLeft);

    // 创建关卡选择按钮
    QHBoxLayout *levelButtonLayout = new QHBoxLayout();
    levelButtonLayout->setSpacing(100);
    QPushButton *level1Button = new QPushButton("关卡一", this);
    QPushButton *level2Button = new QPushButton("关卡二", this);
    QPushButton *level3Button = new QPushButton("关卡三", this);

    // 设置按钮样式
    level1Button->setStyleSheet(buttonStyle);
    level2Button->setStyleSheet(buttonStyle);
    level3Button->setStyleSheet(buttonStyle);
    level1Button->setFixedSize(250, 100);
    level2Button->setFixedSize(250, 100);
    level3Button->setFixedSize(250, 100);
    levelButtonLayout->addWidget(level1Button);
    levelButtonLayout->addWidget(level2Button);
    levelButtonLayout->addWidget(level3Button);
    levelButtonLayout->setContentsMargins(0,0,1400,0);
    mainLayout->addLayout(levelButtonLayout);

    // 连接关卡按钮的点击事件
    connect(level1Button, &QPushButton::clicked, this, &PlayerRankingPage::showLevel1Ranking);
    connect(level2Button, &QPushButton::clicked, this, &PlayerRankingPage::showLevel2Ranking);
    connect(level3Button, &QPushButton::clicked, this, &PlayerRankingPage::showLevel3Ranking);

    // 创建滚动区域并设置滚动条策略
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet("background-color: transparent;");
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizeAdjustPolicy(QScrollArea::AdjustToContents);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidgetResizable(true);  // 自动调整大小
  //  scrollArea->setFixedSize(400, 1100);  // 设置滚动区域的固定宽度和高度

    // 创建滚动区域的内部 widget 和排行榜布局管理器
    QWidget *scrollWidget = new QWidget();
    scrollWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

    rankListLayout = new QVBoxLayout(scrollWidget);
    rankListLayout->setSpacing(10);
    rankListLayout->setAlignment(Qt::AlignTop);

    // 将滚动区域设置为scrollWidget
    scrollArea->setWidget(scrollWidget);

    // 添加排行榜标题
    QHBoxLayout *titleLayout = new QHBoxLayout();
    QLabel *avatarTitle = new QLabel("头像", this);
    avatarTitle->setAlignment(Qt::AlignLeft);
    avatarTitle->setFont(QFont("Microsoft YaHei", 18, QFont::Bold));
    avatarTitle->setStyleSheet("font-weight: bold; color: white");
    titleLayout->addWidget(avatarTitle);
    rankListLayout->addSpacing(40);

    //添加用户名标题
    QLabel *usernameTitle = new QLabel("用户名", this);
    usernameTitle->setFont(QFont("Microsoft YaHei", 18, QFont::Bold));
    usernameTitle->setAlignment(Qt::AlignLeft);
    usernameTitle->setStyleSheet("font-weight: bold; color: white");
    titleLayout->addWidget(usernameTitle);
    QLabel *timeTitle = new QLabel("完成时间", this) ;
    timeTitle->setFont(QFont("Microsoft YaHei", 18, QFont::Bold));
    timeTitle->setStyleSheet("font-weight: bold; color: white");
    titleLayout->addWidget(timeTitle);
    timeTitle->setAlignment(Qt::AlignLeft);
    rankListLayout->addLayout(titleLayout);
    rankListLayout->setContentsMargins(0,0,1200,0);

    // 将滚动区域添加到主布局管理器
    mainLayout->addWidget(scrollArea);
    mainLayout->setContentsMargins(0,0,0,1430);

    // 连接返回按钮的点击事件
    connect(returnButton, &QPushButton::clicked, this, &PlayerRankingPage::returnToMainPage);
}

PlayerRankingPage::~PlayerRankingPage(){}

//返回主页面
void PlayerRankingPage::on_ReturnButton_clicked()
{
    emit returnToMainPage(); // 发出返回父页面信号
}

//读文件
std::vector<UserRankItem> PlayerRankingPage::readRankingFromFile()
{
    std::vector<UserRankItem> userRankList;
    QFile file("./www.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return userRankList;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(" ");
        if (parts.size() >= 3)
        {
            QString username = parts[1];
            QString elapsedTime = parts[2];
            qDebug()<<elapsedTime<<'\n';
            QString imagth=parts[3];
            userRankList.push_back({username, imagth, elapsedTime});
        }
    }
    file.close();
    return userRankList;
}


//分别用于显示关卡一、关卡二和关卡三的排行榜
void PlayerRankingPage::showLevel1Ranking()
{
    updateRanking("关卡一");
}

void PlayerRankingPage::showLevel2Ranking()
{
    updateRanking("关卡二");
}

void PlayerRankingPage::showLevel3Ranking()
{
    updateRanking("关卡三");
}
// 函数：锐化图像
QImage _enhanceContrast(const QImage& inputImage)
{
    QImage resultImage = inputImage.copy(); // 复制输入图像

    // 调整对比度参数
    float contrastFactor = 1.5; // 可根据需要调整

    for (int y = 0; y < resultImage.height(); ++y) {
        for (int x = 0; x < resultImage.width(); ++x) {
            QRgb pixel = inputImage.pixel(x, y);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);

            // 应用对比度调整
            r = qBound(0, static_cast<int>((r - 128) * contrastFactor + 128), 255);
            g = qBound(0, static_cast<int>((g - 128) * contrastFactor + 128), 255);
            b = qBound(0, static_cast<int>((b - 128) * contrastFactor + 128), 255);

            QRgb newPixel = qRgb(r, g, b);
            resultImage.setPixel(x, y, newPixel);
        }
    }

    return resultImage;
}

QImage _sharpenImage(const QImage& inputImage)
{
    QImage resultImage = inputImage.copy(); // 复制输入图像

    // 锐化核
    float kernel[3][3] = {
        { -1, -1, -1 },
        { -1,  9, -1 },
        { -1, -1, -1 }
    };

    int kernelSize = 3;
    int radius = kernelSize / 2;

    // 对每个像素应用锐化核
    for (int y = radius; y < resultImage.height() - radius; ++y) {
        for (int x = radius; x < resultImage.width() - radius; ++x) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int ky = 0; ky < kernelSize; ++ky) {
                for (int kx = 0; kx < kernelSize; ++kx) {
                    QRgb pixel = inputImage.pixel(x + kx - radius, y + ky - radius);
                    sumR += qRed(pixel) * kernel[ky][kx];
                    sumG += qGreen(pixel) * kernel[ky][kx];
                    sumB += qBlue(pixel) * kernel[ky][kx];
                }
            }
            sumR = qBound(0, sumR, 255);
            sumG = qBound(0, sumG, 255);
            sumB = qBound(0, sumB, 255);
            QRgb newPixel = qRgb(sumR, sumG, sumB);
            resultImage.setPixel(x, y, newPixel);
        }
    }

    // 在锐化处理后应用对比度增强
    resultImage = _enhanceContrast(resultImage);

    return resultImage;
}


//先删除再读取然后排序显示出来
void PlayerRankingPage::updateRanking(const QString &level)
{
    // 清空当前排行榜
    QLayoutItem *item;
    while ((item = rankListLayout->takeAt(2)) != nullptr) // 从第3个索引开始，因为前面有标题栏等控件
    {
        if (QLayout *layout = item->layout())
        {
            QLayoutItem *subItem;
            while ((subItem = layout->takeAt(0)) != nullptr)
            {
                if (QWidget *widget = subItem->widget())
                {
                    widget->deleteLater();
                }
                delete subItem;
            }
        }
        delete item;
    }
    // 读取并显示新的排行榜数据
    std::vector<UserRankItem> userRankList = readRankingFromFile(level);
    std::sort(userRankList.begin(), userRankList.end(), [](const UserRankItem &a, const UserRankItem &b)
              {
                  int timeA = a.elapsedTime.split(" ")[0].toInt();
                  int timeB = b.elapsedTime.split(" ")[0].toInt();
                  return timeA < timeB;
              });

    //放入列表
    for (const auto &rankItem : userRankList)
    {
        QHBoxLayout *rankItemLayout = new QHBoxLayout();
        rankItemLayout->setSpacing(100);

        QLabel *avatarLabel = new QLabel(this);
        // avatarLabel->setScaledContents(true);
        // avatarLabel->setPixmap(rankItem.avatarIcon.pixmap(QSize(80, 80))); // 设置头像大小
        // avatarLabel->setFixedSize(80, 80); // 固定头像大小
        // avatarLabel->setStyleSheet("border: 2px solid black; border-radius: 10px;");
        // rankItemLayout->addWidget(avatarLabel);

        // QPixmap avatarPixmap(imagePath);
        // QImage processedImage = sharpenImage(avatarPixmap.toImage());
        // // avatarLabel->setScaledContents(true);
        // // avatarLabel->setPixmap(avatarPixmap.scaled(avatarLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        // // // 更新侧边栏信息

        // avatarLabel->setScaledContents(true);
        // avatarLabel->setPixmap(QPixmap::fromImage(processedImage));

        avatarLabel->setFixedSize(80, 80); // 设置头像大小
        QPixmap avatarPixmap(rankItem.avatarIcon);
        QImage processedImage = _sharpenImage(avatarPixmap.toImage());
        avatarLabel->setScaledContents(true);
        avatarLabel->setPixmap(QPixmap::fromImage(processedImage));
        rankItemLayout->addWidget(avatarLabel);
         // // 更新侧边栏信息

        // avatarLabel->setScaledContents(true);
        // avatarLabel->setPixmap(QPixmap::fromImage(processedImage));
        rankItemLayout->setSpacing(430);
        QLabel *usernameLabel = new QLabel(rankItem.username, this);
        usernameLabel->setFont(QFont("Microsoft YaHei", 16));
        usernameLabel->setStyleSheet("font-weight: bold; color: white");
        usernameLabel->setAlignment(Qt::AlignCenter);
        rankItemLayout->addWidget(usernameLabel);

        rankItemLayout->setSpacing(20);
        QLabel *timeLabel = new QLabel(rankItem.elapsedTime+"秒", this);
        timeLabel->setFont(QFont("Microsoft YaHei", 16));
        timeLabel->setStyleSheet("font-weight: bold; color: white");
        timeLabel->setAlignment(Qt::AlignCenter);
        rankItemLayout->addWidget(timeLabel);

        rankListLayout->addLayout(rankItemLayout);
    }
}

//根据指定的关卡从文件中读取排行榜数据
std::vector<UserRankItem> PlayerRankingPage::readRankingFromFile(const QString &level)
{
    std::vector<UserRankItem> userRankList;
    QFile file("./www.txt");
    qDebug() << "Attempting to open file:" << file.fileName(); // 打印文件路径以调试

    if (!file.exists())
    {
        qDebug() << "File does not exist!";
        return userRankList;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return userRankList;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(" ");
        if (parts.size() >= 3 && parts[0] == level)
        {
            QString username = parts[1];
            QString elapsedTime = parts[2];
            QString imagth=parts[3];
            qDebug()<<imagth<<'\n';
            userRankList.push_back({username, imagth, elapsedTime});
        }
    }
    file.close();
    return userRankList;
}


void PlayerRankingPage::cleanrank()
{
    QLayoutItem *item;
    while ((item = rankListLayout->takeAt(2)) != nullptr) // 从第3个索引开始，因为前面有标题栏等控件
    {
        if (QLayout *layout = item->layout())
        {
            QLayoutItem *subItem;
            while ((subItem = layout->takeAt(0)) != nullptr)
            {
                if (QWidget *widget = subItem->widget())
                {
                    widget->deleteLater();
                }
                delete subItem;
            }
        }
        delete item;
    }
}
