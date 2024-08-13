#ifndef WIDGET_H
#define WIDGET_H

#include "config.h"
#include "PlayerRankingPage.h"
#include "SettingsPage.h"
#include "PracticePage.h"
#include "CompetitionPage.h"
#include "logindialog.h"
#include "sudokugame.h"
#include "musicplayer.h"
#include "ExitDialog.h"
#include "PlayerDataWindow.h"
#include <QMainWindow>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void uploadBackgroundImage();
    void animateButton(QPushButton *button);
    void connectButtonAnimations(QPushButton *button);
    QWidget *cgGalleryPage;
    QPushButton *settingsButton;
    QPushButton *practiceButton;
    QPushButton *competitionButton;
    QPushButton *playerRankingButton;
    QPushButton *loginButton;
     QPushButton *exitButton;

    QString getUsername() const;
    QString getPassword() const;
    QString getAvatarPath() const;

private slots:
    void showSettingsPage();
    void on_ReturnToMainPage();
    void showPracticePage();
    void showCompetitionPage();
    void showPlayerRankingPage();
    void showLoginDialogPage();
    void showsudokugamepage();
    void updateinformation();
    void showmusicPage();
    void onExitButtonClicked();
    void showCGGalleryPage();
    void showPlayerDataWindow();

private:
    Ui::Widget *ui;
    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;
    QStackedWidget *stackedWidget;
    SettingsPage *settingsPage;
    PracticePage *practicePage;
    CompetitionPage *competitionPage;
    PlayerRankingPage *playerRankingPage;
    LoginDialog *loginDialogPage;
    SudokuGame *sudokugame;
    MusicPlayer *musicPlayerpage;
    PlayerDataWindow *playerDataWindow;
private:
    QLabel *announcementLabel;
    QString username;
    QString loginTime;
    QLabel *usernameLabel;
    QLabel *loginTimeLabel;
    QLabel *avatarLabel ;
    QString imagePath;
    QString _password;
    QString _username;
    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;
    QVBoxLayout *layout;
    QPushButton *chatButton;
    QPushButton *sendButton;
    QTextEdit *chatTextEdit;
    QLineEdit *messageLineEdit;
};

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr) : QLabel(parent) {}

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit clicked();
        QLabel::mousePressEvent(event);
    }
};


#endif // WIDGET_H
