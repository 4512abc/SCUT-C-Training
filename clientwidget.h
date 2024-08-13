#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include"LevelOnepa.h"

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr); 
    ~ClientWidget();
    void readyReadHandler();
    QString getLocalIPAddress(); // 声明获取局域网IP地址的函数
    bool eventFilter(QObject *obj, QEvent *event);//事件过滤器
    void sendJoinRequest();//发送对战邀请
    void enterGameMap();//进入游戏地图，并连接信号
    void checkGameOver();//游戏结束后的操作
    LevelOnepa *levelonepa;//联机游戏地图
    QString imagePath;
    QString _password;
    QString _username;
private slots:
    void connectToServer();//连接到服务器
    void disconnectFromServer();//断开连接
    void sendMessage();//发送信息
    void connectedToServer();//已经连接到服务器，更改状态
    void disconnectedFromServer();//断开与服务器的连接要更新的状态
    void displayMessage(const QString &message);//展示信息
    void displayError(const QString &error);//展示错误信息

private:
    QLineEdit *serverAddressLineEdit;//ip地址的输入框
    QLineEdit *serverPortLineEdit;//端口输入框
    QPushButton *connectButton;//连接按钮
    QPushButton *disconnectButton;//取消连接按钮
    QTextEdit *communicationLog;//作为文本框
    QLineEdit *messageLineEdit;//信息输入
    QPushButton *sendButton;//发送按钮
    QLabel *connectionStatusLabel;//连接状态
    QTcpSocket *socket;//一个socket对象
};

#endif
