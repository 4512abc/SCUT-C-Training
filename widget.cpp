#include "widget.h"
#include "./ui_widget.h"
#include "playerdatawindow.h"

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include "SettingsPage.h"
#include "PracticePage.h"
#include<QPropertyAnimation>
#include<QGraphicsView>
#include<QRandomGenerator>
#include<QInputDialog>
#include<QDateTime>
#include<QMenu>
#include<QGraphicsOpacityEffect>
void Widget::showSettingsPage()
{
    stackedWidget->setCurrentIndex(1); // 切换到设置页面
}

void Widget::on_ReturnToMainPage()
{
    stackedWidget->setCurrentIndex(0); // 切换回主页面
}

void Widget::showPracticePage()
{
    stackedWidget->setCurrentIndex(2); // 切换到 PracticePage
}

void Widget::showCompetitionPage()
{
    stackedWidget->setCurrentIndex(3); // 切换到 CompetitionPage
}

void Widget::showPlayerRankingPage()
{
    playerRankingPage->cleanrank();
    stackedWidget->setCurrentIndex(4); // 切换到 PlayerRankingPage
}

void Widget::showLoginDialogPage()
{
    stackedWidget->setCurrentIndex(5);
}

void Widget::showsudokugamepage()
{
    stackedWidget->setCurrentIndex(6);
}

void Widget::showmusicPage()
{
    stackedWidget->setCurrentIndex(7);
}
void Widget::showCGGalleryPage()
{
    stackedWidget->setCurrentIndex(8);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , practicePage(new PracticePage(this))
    , stackedWidget(new QStackedWidget(this))
    , settingsPage(new SettingsPage(this))
    , competitionPage(new CompetitionPage(this))
    , playerRankingPage(new PlayerRankingPage(this))
    ,loginDialogPage(new LoginDialog(this)),
    sudokugame(new SudokuGame(this)),
    musicPlayerpage(new MusicPlayer(this))
{
    // 窗口设置
    settingsPage->setVisible(false);
    setWindowTitle("植此青绿");
    setWindowIcon(QIcon(":/new/images/ting.png"));

    // 设置窗口为全屏
    setWindowState(Qt::WindowFullScreen);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    // 背景设置

    QImage background(":/new/images/94dbf2c25da5df4c01031cf128ef0ab.png");
    background = background.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(background));
    this->setPalette(palette);

    const QString baseButtonStyle = "QPushButton {"
                                    "background-color: transparent; border: none; border-radius: 0px; "
                                    "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                                    "}"
                                    "QPushButton:hover {"
                                    "color: lightgray;"  // 悬停时的颜色变化
                                    "}";
    //创建按钮
    QPushButton *settingsButton = new QPushButton("帮助", this);
    QPushButton *practiceButton = new QPushButton("练习模式", this);
    QPushButton *competitionButton = new QPushButton("比赛模式", this);
    QPushButton *playerRankingButton = new QPushButton("玩家排行", this);
    QPushButton *shuduButton=new QPushButton("数独游戏", this);
    QPushButton *musicButton=new QPushButton("音乐小窝",this);
    exitButton = new QPushButton("退出", this);

    // 连接退出按钮点击事件
    connect(exitButton, &QPushButton::clicked, this, &Widget::onExitButtonClicked);

    //为每个按钮添加背景图片样式
    settingsButton->setStyleSheet(baseButtonStyle);

    settingsButton->setFixedSize(200, 75);

    practiceButton->setStyleSheet(baseButtonStyle);
    practiceButton->setFixedSize(200, 75);

    competitionButton->setStyleSheet(baseButtonStyle);

    competitionButton->setFixedSize(200, 75);

    playerRankingButton->setStyleSheet(baseButtonStyle);

    playerRankingButton->setFixedSize(200, 75);

    shuduButton->setStyleSheet(baseButtonStyle);
    shuduButton->setFixedSize(200, 75);

    musicButton->setStyleSheet(baseButtonStyle);
    musicButton->setFixedSize(200, 75);

    exitButton->setStyleSheet(baseButtonStyle);
    exitButton->setFixedSize(200, 75);

    // 创建布局
  //  QVBoxLayout *vlayout = new QVBoxLayout(this);
   // this->setLayout(vlayout); // 添加这行代码，使vlayout成为主窗口的布局管理器
  //  vlayout->setAlignment(Qt::AlignCenter); // 修改对齐方式为居中对齐
    QVBoxLayout *hlayout = new QVBoxLayout();
    hlayout->setSpacing(0); // 修改按钮间距
    hlayout->addWidget(settingsButton);
    hlayout->addWidget(practiceButton);
    hlayout->addWidget(competitionButton);
    hlayout->addWidget(playerRankingButton);
    hlayout->addWidget(shuduButton);
    hlayout->addWidget(musicButton);
    hlayout->addWidget(exitButton);
    hlayout->setSpacing(1); // 按钮之间的间距
    hlayout->setContentsMargins(130, 470, 0, 1500); // 设置上边距为300像素
    loginButton = new QPushButton("登录", this);
    loginButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");
    // 设置登录按钮的位置和大小
    loginButton->setFixedSize(200, 75);
    // vlayout->addWidget(loginButton, 0, Qt::AlignLeft | Qt::AlignTop);

    // vlayout->addLayout(hlayout);

    // 将rightBottomLayout添加到布局底部
    QHBoxLayout *rightBottomLayout = new QHBoxLayout();
    rightBottomLayout->addStretch();
    const QString _baseButtonStyle = "QPushButton {"
                                    "background-color: transparent; border: none; border-radius: 0px; "
                                    "font-family: 'Microsoft YaHei'; font-size: 25px; font-weight: bold; color: white;"
                                    "}"
                                    "QPushButton:hover {"
                                    "color: lightgray;"  // 悬停时的颜色变化
                                    "}";
    QPushButton*newButton = new QPushButton("个人信息", this);
    newButton->setStyleSheet(_baseButtonStyle);  // 根据需要设置样式表
    newButton->setFixedSize(100, 50);           // 设置按钮大小

    // 初始化侧边栏
    usernameLabel = new QLabel(_username, this);
    loginTimeLabel = new QLabel(this);
    avatarLabel = new QLabel(this); // 添加头像标签
    avatarLabel->setFixedSize(230, 150); // 设置头像大小

    // 设置侧边栏样式
    usernameLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 25px; font-weight: bold; color: white;");
    loginTimeLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 25px; font-weight: bold; color: white;");

    // 布局侧边栏
    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    sidebarLayout->setSpacing(5); // 设置控件之间的默认间距
    sidebarLayout->setContentsMargins(0, 0, 0, 800); // 设置布局的边距
    sidebarLayout->addWidget(usernameLabel);
    sidebarLayout->addWidget(loginTimeLabel);
    sidebarLayout->addWidget(avatarLabel); // 添加头像标签到布局中
    sidebarLayout->addWidget(newButton);

    rightBottomLayout->addLayout(sidebarLayout);

    // 美化整个侧边栏
    QWidget *sidebarWidget = new QWidget(this);
    sidebarWidget->setLayout(sidebarLayout);
    sidebarWidget->setStyleSheet(
        "QWidget {"
        "border: 2px solid red; "    // 设置整个侧边栏的边框
        "border-radius: 10px; "        // 设置圆角
        "background-color: rgba(0, 0, 0, 0.5); " // 设置背景色和透明度
        "padding: 10px; "              // 内边距
        "}"
        );
    rightBottomLayout->addWidget(sidebarWidget);


    // vlayout->addLayout(rightBottomLayout);
    // rightBottomLayout->setContentsMargins(330, 360, 500, 0);  // 设置边距，使其靠近右上角
    // 添加主页面到QStackedWidget
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    // mainLayout->setAlignment(Qt::AlignCenter);
    // mainLayout->addSpacing(100);
    mainLayout->addWidget(loginButton, 0, Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addLayout(hlayout);
    mainLayout->setSpacing(100);
    mainLayout->addLayout(rightBottomLayout);
    rightBottomLayout->setContentsMargins(650, 0, 0, 1300);
    mainLayout->addStretch(1);  // 添加一个可伸缩的空间，使得布局在垂直方向上居中

   // mainLayout->setContentsMargins(20, 20, 800, 160);
    // QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 0,0, 10);
    // mainLayout->setSpacing(20);

    // // 添加返回按钮在左上角
    // mainLayout->addWidget(returnButton, 0, Qt::AlignTop | Qt::AlignLeft);

    // // 添加内容
    // mainLayout->addLayout(imageLayout);

    // // 添加具体说明
    // mainLayout->addWidget(noTreeLabel5);

    // // 设置主布局
     setLayout(mainLayout);
    QWidget *mainPage = new QWidget(this);
    mainPage->setLayout(mainLayout);
    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->addWidget(practicePage);
    stackedWidget->addWidget(competitionPage);
    stackedWidget->addWidget(playerRankingPage);
    stackedWidget->addWidget(loginDialogPage);
    stackedWidget->addWidget(sudokugame);
    stackedWidget->addWidget(musicPlayerpage);

    // 连接按钮点击事件
    connect(settingsButton, &QPushButton::clicked, this, &Widget::showSettingsPage);
    connect(practiceButton, &QPushButton::clicked, this, &Widget::showPracticePage);
    connect(competitionButton, &QPushButton::clicked, this, &Widget::showCompetitionPage);
    connect(playerRankingButton, &QPushButton::clicked, this, &Widget::showPlayerRankingPage);
    connect(shuduButton,&QPushButton::clicked, this, &Widget::showsudokugamepage);
    connect(musicButton,&QPushButton::clicked,this,&Widget::showmusicPage);

    // 连接返回主页面信号
    connect(playerRankingPage, &PlayerRankingPage::returnToMainPage, this, &Widget::on_ReturnToMainPage);
    connect(settingsPage, &SettingsPage::returnToMainPage, this, &Widget::on_ReturnToMainPage);
    connect(practicePage, &PracticePage::returnToMainPage, this, &Widget::on_ReturnToMainPage);
    connect(competitionPage, &CompetitionPage::returnToMainPage, this, &Widget::on_ReturnToMainPage);
    connect(sudokugame,&SudokuGame::returnToMainPage,this,&Widget::on_ReturnToMainPage);
    connect(musicPlayerpage,&MusicPlayer::returnToMainPage,this,&Widget::on_ReturnToMainPage);
    connect(newButton, &QPushButton::clicked, this, &Widget::showPlayerDataWindow);

    // 设置按钮大小
    connect(loginButton, &QPushButton::clicked, this, &Widget::showLoginDialogPage);
    connect(loginDialogPage, &LoginDialog::returnToMainPage, this, &Widget::on_ReturnToMainPage);
    connect(loginDialogPage, &LoginDialog::returnToMainPage, this, &Widget::updateinformation);

    settingsButton->setToolTip("打开设置页面");
    practiceButton->setToolTip("进入练习模式");
    competitionButton->setToolTip("进入比赛模式");
    playerRankingButton->setToolTip("查看玩家排行");
    shuduButton->setToolTip("打开数独游戏");
    musicButton->setToolTip("进入音乐小窝");

    musicPlayerpage->play();
    updateinformation();
}

Widget::~Widget()
{
    delete ui;
}

//图片锐化技术
QImage enhanceContrast(const QImage& inputImage)
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

QImage sharpenImage(const QImage& inputImage)
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
    resultImage = enhanceContrast(resultImage);

    return resultImage;
}



//更新信息,并实现侧边栏功能
void Widget::updateinformation()
{
    if(loginDialogPage->getUsername()=="")
    {
        this->_username="游客登入";
    }
    else
    {
    this->_username=loginDialogPage->getUsername();
    }
    if(_username=="游客登入")
    {
         usernameLabel->setText("当前状态: "+_username);
    }
    else
    {
            usernameLabel->setText("用户名: "+_username);
    }
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString loginTimeText = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
    loginTimeLabel->setText(loginTimeText);
    this->_password=loginDialogPage->getPassword();
    if(loginDialogPage->getAvatarPath()=="")
    {
        QStringList imagePaths =
        {
            ":/new/images/background3.png",
            ":/new/images/background4.png",
            ":/new/images/background5.jpg",
            ":/new/images/background6.png",
            ":/new/images/background7.png",
            ":/new/images/background8.jpg",
            ":/new/images/background11.png",
            ":/new/images/background12.png",
            ":/new/images/gdyg.png",
            ":/new/images/gdyg1.png",
            ":/new/images/hx.png",
            ":/new/images/hx2.png",
            ":/new/images/hx3.png",
            ":/new/images/hx4.png",
            ":/new/images/hx5.png",
            ":/new/images/hx6.png",
            ":/new/images/stl1.png",
            ":/new/images/stl2.png",
            ":/new/images/stl3.png",
            ":/new/images/stl4.png",
            ":/new/images/stl6.png",
            ":/new/images/stl7.png"
        };
        // 生成一个0到imagePaths.size()-1之间的随机数
        int randomIndex = QRandomGenerator::global()->bounded(imagePaths.size());
        // 设置随机选择的图片路径
        this->imagePath = imagePaths[randomIndex];
        //设置默认图片
    }
    else
    {
        this->imagePath=loginDialogPage->getAvatarPath();
    }
    QPixmap avatarPixmap(imagePath);
    QImage processedImage = sharpenImage(avatarPixmap.toImage());
    avatarLabel->setScaledContents(true);
    avatarLabel->setPixmap(QPixmap::fromImage(processedImage));

    practicePage->_password=_password;
    practicePage->_username=_username;
    practicePage->imagePath=imagePath;
    competitionPage->_password=_password;
    competitionPage->_username=_username;
    competitionPage->imagePath=imagePath;
    //更新练习界面的用户名
    practicePage->updateinformation();
}

// 新添加的函数，可以返回用户名，密码和图像路径
QString Widget::getUsername() const
{
    return _username;
}

QString Widget::getPassword() const
{
    return _password;
}

QString Widget::getAvatarPath() const
{
    return imagePath;
}


void Widget::uploadBackgroundImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择背景图片"), "", tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp)"));
    if (!filePath.isEmpty())
    {
        QImage backgroundImage(filePath);
        backgroundImage = backgroundImage.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(this->backgroundRole(), QBrush(backgroundImage));
        this->setPalette(palette);
        this->update(); // 更新窗口以应用更改
    }
}

void Widget::animateButton(QPushButton *button)
{
    QPropertyAnimation *animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(300);
    QRect startRect = button->geometry();
    QRect endRect = QRect(startRect.x() - 10, startRect.y() - 10, startRect.width() + 20, startRect.height() + 20);
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void Widget::connectButtonAnimations(QPushButton *button)
{
    connect(button, &QPushButton::pressed, [=](){ animateButton(button); });
    connect(button, &QPushButton::released, [=](){ animateButton(button); });
}

void Widget::onExitButtonClicked()
{
    ExitDialog exitDialog(this);
    if (exitDialog.exec() == QDialog::Accepted)
    {
        QApplication::quit();
    }
}

void Widget::showPlayerDataWindow()
{
        playerDataWindow = new PlayerDataWindow(this);
        playerDataWindow->setPlayerInfo(getUsername(), getAvatarPath());
        playerDataWindow->readGameData(getUsername());
        playerDataWindow->show();
}
