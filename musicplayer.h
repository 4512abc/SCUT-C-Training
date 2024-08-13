#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QFileDialog>
#include <QListWidget>
#include<QTextEdit>
#include<QScrollArea>
#include<QPainter>

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();
    void initMusic() ;
    QLabel *imageLabel;

public slots:
void play();
private slots:
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void setPosition(int position);
    void showLyrics();
    void importFile();
    void setButtonStyle(QPushButton *button);
    void setButtonStyle(QPushButton *button, const QString &buttonClass);
    void displayMusicInfo(const QString &filePath);
    bool eventFilter(QObject *obj, QEvent *event);
    void on_ReturnButton_clicked();

signals:
    void returnToMainPage(); // 发出返回主页面信号

private:
    QListWidgetItem* createPlaylistItem(const QString &filePath,bool isLocalFile); // 新增的函数声明
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    QPushButton* playButton;
    QPushButton* showLyricsButton;
    QPushButton* stopButton;
    QPushButton* openButton;
    QPushButton *importButton;
    QSlider* positionSlider;
    QLabel* durationLabel;
    QListWidget* playlistWidget;
    QTextEdit* lyricsTextEdit;
    QScrollArea* lyricsScrollArea;
    QPushButton *localMusicButton;
    QPushButton *nowPlayingButton;
    QPushButton *favoritesButton;
    QPushButton *playlistsButton;
    bool isPlaying;
    QPixmap defaultAlbumArt;
    QMediaPlayer *videoPlayer;
};

#endif // MUSICPLAYER_H
