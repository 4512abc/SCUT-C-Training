#ifndef CHATROOMWINDOW_H
#define CHATROOMWINDOW_H

#include <QWidget>
#include "clientwidget.h"
#include "serverwidget.h"

class ChatRoomWindow : public QWidget
{
    Q_OBJECT
    //使得类可以发送和接受信号并与槽连接，同时也可以实现对对象的父子关系管理以及自动的内存管理
    //既可以用于对象之间的通信，也可以防止内存泄漏并且简化内存管理

public:
    ChatRoomWindow(QWidget *parent = nullptr);
    ~ChatRoomWindow();
    QString imagePath;
    QString _password;
    QString _username;

signals:
    void returnToCompetitionPage();

private slots:
    void on_connectServerButton_clicked();
    void on_startServerButton_clicked();
    void on_ReturnButton_clicked();

private:
    ClientWidget *clientWidget;
    ServerWidget *serverWidget;
};

#endif
