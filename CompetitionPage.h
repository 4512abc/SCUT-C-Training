#ifndef COMPETITIONPAGE_H
#define COMPETITIONPAGE_H

#include "config.h"
#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include "HelpWindow.h"
#include "FreeModePage.h"
#include "InputModePage.h"
#include <QStackedWidget>
#include "chatroomwindow.h"

class CompetitionPage : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionPage(QWidget *parent = nullptr);
    ~CompetitionPage();

    QPushButton *returnButton;
    QPushButton *freeModeButton;
    QPushButton *inputModeButton;
    QPushButton *helpButton;
    QPushButton *charButton;

    void updateInformation();
    //更新下面三个信息使用
    QString imagePath;
    QString _password;
    QString _username;

private slots:
    void on_ReturnButton_clicked();
    void on_ReturnToCompetitionPage();
    void showFreeModePage();
    void showInputModePage();
    void showHelpWindow();
    void showCompetitionPage();
    void showChatRoom();
    //各种页面切换，返回函数

signals:
    void returnToMainPage(); // 发出返回主页面信号

private:
    QVBoxLayout *vLayout;
    //主布局
    FreeModePage *freeModePage;
    //自由模式
    InputModePage *inputModePage;
    //输入模式
    ChatRoomWindow *chatRoom;
    //聊天室
    HelpWindow *helpWindow;
    QStackedWidget *stackedWidget;
    //堆栈进行页面管理
};

#endif
