#include "musicplayer.h"
#include "qscrollbar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTime>
#include <QDir>
#include <QFileDialog>
#include <QPalette>
#include<QEvent>
#include<QGraphicsOpacityEffect>
#include<QPropertyAnimation>
#include<QMenu>
#include<QInputDialog>

MusicPlayer::MusicPlayer(QWidget *parent)
    : QWidget(parent), player(new QMediaPlayer(this)), audioOutput(new QAudioOutput(this)), isPlaying(false)
{

    //这里将播放器的音频输出设置为audioOutput对象
    player->setAudioOutput(audioOutput);
    //使得背景变为透明色
    QPalette palette;
    palette.setBrush(QPalette::Base, QBrush(QPixmap(":/../../Desktop/1144676720563d4798e390f7866dc01.png"))); // 替换为你的背景图片路径
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    //设置图片，并安装事件过滤器
    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);
    QPixmap image(":/new/images/music.png");
    imageLabel->setPixmap(image);
    imageLabel->setFixedSize(600,700);
    imageLabel->installEventFilter(this);

    // 左侧菜单布局
    QVBoxLayout *menuLayout = new QVBoxLayout();

    //中间歌词区域
    lyricsTextEdit = new QTextEdit(this);
    lyricsTextEdit->setReadOnly(true);
    lyricsTextEdit->setStyleSheet("QTextEdit { background: transparent; color: white; border: 1px solid #ccc; border-radius: 5px; font-size: 18px; }"); // 修改字体大小和边框样式
    lyricsTextEdit->setAlignment(Qt::AlignCenter);

    QString buttonStyle =(   "QPushButton {"
                           "background-color: transparent; border: 2px solid white; border-radius: 10px; "
                           "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                           "}"
                           "QPushButton:hover {"
                           "color: lightgray;"  // 悬停时的颜色变化
                           "}");

    // 底部控制区域
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    importButton = new QPushButton(this);
    playButton = new QPushButton(this);
    showLyricsButton = new QPushButton(this);

    //返回按钮
    QPushButton *returnButton = new QPushButton("返回", this);
    //水平滑块部件和一个标签部件，就是一个进度条和时间
    positionSlider = new QSlider(Qt::Horizontal, this);
    durationLabel = new QLabel("00:00 / 00:00", this);

    // 设置按钮图标和文字
    importButton->setText("导入歌曲");
    importButton->setStyleSheet(buttonStyle);
    importButton->setIcon(QIcon(":/new/images/upload.png"));
    importButton->setIconSize(QSize(30, 30));

    //设置播放按钮
    playButton->setText("播放");
    playButton->setStyleSheet(buttonStyle);
    playButton->setIcon(QIcon(":/new/images/stop.png"));
    playButton->setIconSize(QSize(30, 30));

    //显示歌词按钮
    showLyricsButton->setText("显示歌词");
    showLyricsButton->setStyleSheet(buttonStyle);
    showLyricsButton->setIcon(QIcon(":/new/images/cihui.png"));
    showLyricsButton->setIconSize(QSize(30, 30));

    //设置返回按钮
    returnButton->setStyleSheet(buttonStyle);
    returnButton->setIcon(QIcon(":/new/images/return.png"));
    returnButton->setIconSize(QSize(30,30));

    //将他们添加到底部的水平布局内部
    controlsLayout->addWidget(importButton);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(showLyricsButton);
    controlsLayout->addWidget(returnButton);
    controlsLayout->addWidget(positionSlider);
    controlsLayout->addWidget(durationLabel);

    // 初始化 playlistWidget
    playlistWidget = new QListWidget(this);
    playlistWidget->setFont(QFont("Arial", 12)); // 设置列表字体大小
    defaultAlbumArt = QPixmap(":/new/images/music.png"); // 设置一个默认专辑封面图像

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *centralLayout = new QHBoxLayout();
    centralLayout->addLayout(menuLayout);
    centralLayout->addWidget(imageLabel);
    centralLayout->addWidget(lyricsTextEdit);
    // 在构造函数中为歌词文本框添加阴影效果
    lyricsTextEdit->setStyleSheet("QTextEdit { background: transparent; color: white; border: 1px solid #ccc; border-radius: 5px; font-size: 18px; }"
                                  "QTextEdit::hover { border: 1px solid #999; }"
                                  "QTextEdit::focus { border: 1px solid #666; }"
                                  "QTextEdit {"
                                  "    background-color: rgba(0, 0, 0, 100);"
                                  "    color: white;"
                                  "    border: 1px solid #ccc;"
                                  "    border-radius: 5px;"
                                  "    font-size: 18px;"
                                  "    padding: 10px;"
                                  "    box-shadow: 0 0 10px rgba(0, 0, 0, 0.5);"
                                  "}");

    // 动态排版：调整歌词文本框的大小和位置以适应界面大小
    QRect centralRect = centralLayout->geometry();
    int lyricsWidth = centralRect.width() * 0.8; // 歌词文本框宽度为中间布局的80%
    int lyricsHeight = centralRect.height() * 0.7; // 歌词文本框高度为中间布局的70%
    int lyricsX = (centralRect.width() - lyricsWidth) / 2; // 歌词文本框水平居中
    int lyricsY = (centralRect.height() - lyricsHeight) / 2; // 歌词文本框垂直居中
    lyricsTextEdit->setGeometry(lyricsX, lyricsY, lyricsWidth, lyricsHeight);

    mainLayout->addLayout(centralLayout);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(playlistWidget);
    mainLayout->setContentsMargins(0,10,0,1100);
    connect(importButton, &QPushButton::clicked, this, &MusicPlayer::importFile);
    connect(playButton, &QPushButton::clicked, this, &MusicPlayer::play);
    connect(showLyricsButton, &QPushButton::clicked, this, &MusicPlayer::showLyrics);
    connect(positionSlider, &QSlider::sliderMoved, this, &MusicPlayer::setPosition);
    connect(player, &QMediaPlayer::positionChanged, this, &MusicPlayer::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateDuration);
    connect(playlistWidget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem* item){
        player->setSource(QUrl::fromLocalFile(item->data(Qt::UserRole).toString()));
        player->play();
    });
    audioOutput->setVolume(100);
    // 设置按钮样式
    initMusic();
    // 在构造函数中为歌词文本框设置滚动条样式
    lyricsTextEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {"
                                                       "    border: none;"
                                                       "    background: transparent;"
                                                       "    width: 10px;"
                                                       "    margin: 0px 0px 0px 0px;"
                                                       "}"
                                                       "QScrollBar::handle:vertical {"
                                                       "    background: #7ec8e3; /* 浅蓝色 */"
                                                       "    min-height: 20px;"
                                                       "    border-radius: 10px; /* 圆形 */"
                                                       "    border: 2px solid #3f90e3; /* 边框 */"
                                                       "}"
                                                       "QScrollBar::handle:vertical:hover {"
                                                       "    background: #72b4d1; /* 鼠标悬停时颜色变化 */"
                                                       "}"
                                                       "QScrollBar::add-line:vertical {"
                                                       "    height: 0px;"
                                                       "    subcontrol-position: bottom;"
                                                       "    subcontrol-origin: margin;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical {"
                                                       "    height: 0px;"
                                                       "    subcontrol-position: top;"
                                                       "    subcontrol-origin: margin;"
                                                       "}"
                                                       "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                                                       "    width: 0px;"
                                                       "    height: 0px;"
                                                       "    background: none;"
                                                       "}"
                                                       "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                                       "    background: none;"
                                                       "}");

    //初始时候就显示歌词
    showLyrics();
    connect(returnButton, &QPushButton::clicked, this, &MusicPlayer::returnToMainPage);
}
MusicPlayer::~MusicPlayer() {}

//导入音乐
void MusicPlayer::importFile()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "导入音乐文件", "", "音频文件 (*.mp3 *.wav *.ogg)");
    int index = (playlistWidget->count()+1 ); // 初始索引
    for (const QString &file : files)
    {
        if (!file.isEmpty())
        {
            QString songName = QFileInfo(file).fileName(); // 获取文件名作为歌曲名
            QString artist = "Unknown";
            QString album = "Unknown Album";
            QListWidgetItem *item = createPlaylistItem(file, true); // 设置为本地文件路径
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
            QString songInfo = QString::number(index++) + ". " + songName + " - " + artist + " (" + album + ")";
            item->setText(songInfo);
            // 添加间距
            QListWidgetItem *spacer = new QListWidgetItem;
            spacer->setSizeHint(QSize(10, 40)); // 调整间距大小
            playlistWidget->addItem(item);
        }
    }
}

//添加音乐
QListWidgetItem* MusicPlayer::createPlaylistItem(const QString &filePath, bool isLocalFile)
{
    // 创建列表项
    QListWidgetItem *item = new QListWidgetItem(QFileInfo(filePath).fileName());
    item->setData(Qt::UserRole, filePath);
    if (isLocalFile)
    {
        player->setSource(QUrl::fromLocalFile(filePath));
    } else
    {
        // 如果是资源路径，则使用 QUrl 来设置播放器的源文件
        player->setSource(QUrl(filePath));
    }
    return item;
}

void MusicPlayer::displayMusicInfo(const QString &filePath)
{
    // 模拟音乐信息
    QString musicInfo = "歌曲名: Light of Aidan - Lament\n"
                        "专辑名: 华尔街之狼\n"
                        "艺术家: Light of Aidan\n"
                        "时长: 04:30\n"
                        "文件路径: " + filePath;

    lyricsTextEdit->setText(musicInfo);
}

//暂停函数的实现
void MusicPlayer::play()
{
    if (isPlaying)
    {
        player->pause();
    }
    else
    {
        player->play();
    }
    isPlaying = !isPlaying;
}


void MusicPlayer::showLyrics()
{
    // 歌词文本
    QString lyricsText = "曾经绘于心的地图\n"
                         "如同迷路了一般 总是处处碰壁\n"
                         "谁都一样 都是在受伤时\n"
                         "闭上自己的心扉哭泣着吧\n"
                         "一直故作坚强 让自己奋起直追\n"
                         "在这比肩接踵的人群中 仰望那遥远的天空\n"
                         "抛弃过去的只言片语 打开那未来的一页吧\n"
                         "在失足倒地后 慢慢坚强起来\n"
                         "在这世间 有一个被称为「你」的奇迹\n"
                         "让这黑白两色的世界变得五彩斑斓\n"
                         "若能找到活下去的意义\n"
                         "从中描绘出的未来 便是源自于你的勇气\n"
                         "无法好好前进 变得胆怯懦弱\n"
                         "就算心中的真实逐渐迷失\n"
                         "也绝不会放手 绝不会忘怀\n"
                         "那份热切的思念 坚持一步步前行\n"
                         "追逐着梦想的少年 每一天都闪耀着光辉\n"
                         "谁都会拥抱着 那不变的当下生活下去\n"
                         "寂寞的夜晚里 也会感到和某人心心相印\n"
                         "在心中的某处 一直这样坚信不疑\n"
                         "在这世间 有一种被称为「明天」的未来\n"
                         "将小小的泪珠变化为大大的花儿\n"
                         "若能说出活下去的意义\n"
                         "就一定会超越昨日的自己\n"
                         "理想与憧憬太过宏大 变得举步维艰之时\n"
                         "就用那毫无掩饰的心向你细细诉说吧\n"
                         "在刻意绕远的道路某处 在没有尽头的道路某处\n"
                         "也许会再次 和那时的笑颜相遇吧\n"
                         "在这世间 有一个被称为「你」的奇迹\n"
                         "带给了我永不消逝的希望\n"
                         "那活下去的意义 一直都在这心中哦\n"
                         "描绘的未来 就在不远的前方\n"
                         "在这世间 有一道被称为「你」的光芒\n"
                         "在这小小的地球上不断扩大 照亮了黑暗\n"
                         "彼此相连的青空之下 来吧 用自己独有的方式前进\n"
                         "将这起始的诗篇 赠与迈出步伐的你\n";

    lyricsTextEdit->setText(lyricsText);

    //创建一个透明度效果对象，并将其添加
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    lyricsTextEdit->setGraphicsEffect(effect);

    //创建属性动画对象
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(1000);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped); // 设置为黑色
}

//更新播放器的当前播放位置。
void MusicPlayer::updatePosition(qint64 position)
{
    positionSlider->setValue(static_cast<int>(position));
    QTime currentTime((position / 3600000) % 60, (position / 60000) % 60, (position / 1000) % 60);
    QTime totalTime((player->duration() / 3600000) % 60, (player->duration() / 60000) % 60, (player->duration() / 1000) % 60);
    QString format = "mm:ss";
    if (player->duration() > 3600000)
        format = "hh:mm:ss";
    durationLabel->setText(currentTime.toString(format) + " / " + totalTime.toString(format));
}

//更新播放进度
void MusicPlayer::updateDuration(qint64 duration)
{
    positionSlider->setRange(0, static_cast<int>(duration));
}

//设置播放进度
void MusicPlayer::setPosition(int position)
{
    player->setPosition(position);
}

void MusicPlayer::setButtonStyle(QPushButton *button)
{
    button->setIconSize(QSize(32, 32));
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 4px; }"
                          "QPushButton::hover { background-color: #0b7dda; }"
                          "QPushButton::menu-indicator { image: none; }"); // Remove arrow indicator

    if (button == playButton)
    {
        button->setText("播放/暂停");
    }
    else if (button == showLyricsButton)
    {
        button->setIcon(QIcon(":/new/images/music.png")); // Lyrics icon
        button->setText("显示歌词");
    }
}

//设置按钮样式
void MusicPlayer::setButtonStyle(QPushButton *button, const QString &buttonClass)
{
    button->setStyleSheet(
        "QPushButton." + buttonClass + " {"
                                       "    background-color: #2196F3;"
                                       "    color: white;"
                                       "    border: none;"
                                       "    padding: 15px 32px;"
                                       "    text-align: center;"
                                       "    text-decoration: none;"
                                       "    display: inline-block;"
                                       "    font-size: 30px;"
                                       "    margin: 4px 2px;"
                                       "    cursor: pointer;"
                                       "    border-radius: 4px;"
                                       "}"
                                       "QPushButton." + buttonClass + ":hover {"
                        "    background-color: #0b7dda;"
                        "}"
                        "QPushButton." + buttonClass + "::menu-indicator {"
                        "    image: none;"
                        "}"
        );
}

//初始化音乐（initMusic()函数）：
void MusicPlayer::initMusic()
{
    //清空播放列表
    playlistWidget->clear();
    int index = 1; // 初始索引
    // 添加10首音乐到播放列表
    for (int i = 0; i < 1; ++i)
    {
        QString filePath = "qrc:/new/images/yue.wav"; // 绝对路径
        QString songName = "今、歩き出す君へ";
        QString artist = "Ceui"; // 添加歌手信息
        QString album = "PCゲーム「いますぐお兄ちゃんに妹だっていいたい!」ボーカルアルバム"; // 添加专辑信息
        QListWidgetItem *item = createPlaylistItem(filePath, false); // 设置为本地文件路径
        // 添加歌曲序号
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
        QString songInfo = (QString::number(index++)) + ". " + songName + " - " + artist + " (" + album + ")";
        item->setText(songInfo);
        // 添加间距
        QListWidgetItem *spacer = new QListWidgetItem;
        spacer->setSizeHint(QSize(10, 40)); // 调整间距大小
        playlistWidget->addItem(item);
    }
}

//弹出文件对话框让用户选择图像上传
bool MusicPlayer::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == imageLabel && event->type() == QEvent::MouseButtonPress)
    {
        QString filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "图像文件 (*.png *.jpg *.jpeg)");
        if (!filePath.isEmpty()) {
            QPixmap newImage(filePath);
            imageLabel->setPixmap(newImage);
        }
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

//返回按钮
void MusicPlayer::on_ReturnButton_clicked()
{
    emit returnToMainPage(); // 发出返回父页面信号
}
