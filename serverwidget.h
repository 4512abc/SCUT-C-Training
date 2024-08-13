#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include<QProgressBar>
#include"LevelOnepa.h"
class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();
    bool eventFilter(QObject *obj, QEvent *event);
    void displayMessage(const QString &message);
    QString selectedFilePath;
    QString getLocalIPAddress(); // 声明获取局域网IP地址的函数
    LevelOnepa *levelonepa;
    QString imagePath;
    QString _password;
    QString _username;

private slots:
    void startServer();
    void stopServer();
    void sendMessage();
    void incomingConnection();
    void readyReadHandler();
    void selectFile();
    void sendFile(const QString &fileName);
    void disconnectedHandler();
private:
    QLineEdit *portLineEdit;
    QPushButton *startButton;
    QPushButton *stopButton;
    QTextEdit *communicationLog;
    QLineEdit *messageLineEdit;
    QPushButton *sendButton;
    QLabel *serverStatusLabel;
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    QLineEdit *hostLineEdit;
    QPushButton*fileTransferButton;
    QProgressBar *progressBar;
    void handleJoinRequest(QTcpSocket *clientSocket);
    void sendGameOver(QTcpSocket *clientSocket);
};

#endif // SERVERWIDGET_H
