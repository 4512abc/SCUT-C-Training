#pragma execution_character_set("utf-8") // 设置执行字符集为 UTF-8
#include "clientwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QTcpSocket>
#include <QKeyEvent>
#include<QNetworkInterface>
#include<QMessageBox>
#include "LevelOnepa.h"
ClientWidget::ClientWidget(QWidget *parent) : QWidget(parent)
{
    // 设置窗口标题和背景等一系列
    setWindowTitle("客户端");
    setWindowIcon(QIcon(":/new/images/hx.png"));
    resize(1256, 707);

    // 设置透明背景
    QImage background(":/new/images/94dbf2c25da5df4c01031cf128ef0ab.png");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    background = background.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio);
    palette.setBrush(this->backgroundRole(), QBrush(background));
    this->setPalette(palette);

    // 获取本地局域网IP地址
    QString hostAddress = getLocalIPAddress();

    // 创建服务器地址输入框
    serverAddressLineEdit = new QLineEdit(hostAddress, this);
    serverAddressLineEdit->setPlaceholderText("服务器地址");

    //创建服务器端口输入框
    serverPortLineEdit = new QLineEdit("1200", this); // 默认端口号为 1234
    serverPortLineEdit->setPlaceholderText("服务器端口");

    // 创建连接按钮
    connectButton = new QPushButton("连接", this);
    connectButton->setObjectName("ConnectButton");
    connect(connectButton, &QPushButton::clicked, this, &ClientWidget::connectToServer);

    // 创建断开连接按钮
    disconnectButton = new QPushButton("取消按钮", this);
    disconnectButton->setEnabled(false);
    disconnectButton->setObjectName("DisconnectButton");
    connect(disconnectButton, &QPushButton::clicked, this, &ClientWidget::disconnectFromServer);

    // 创建通信日志显示框
    //注意，通信日志框要是可读的
    communicationLog = new QTextEdit(this);
    communicationLog->setObjectName("CommunicationLog");
    communicationLog->setReadOnly(true);

    // 创建消息输入框，并设置占位符文本和最小高度
    messageLineEdit = new QLineEdit(this);
    messageLineEdit->setPlaceholderText("畅所欲言罢");
    messageLineEdit->setMinimumHeight(50);

    //安装事件过滤器来处理键盘事件，让用户操作更便捷
    messageLineEdit->installEventFilter(this);

    // 创建发送按钮，并设置对象名称和槽函数连接
    sendButton = new QPushButton("发送", this);
    sendButton->setEnabled(false);
    sendButton->setObjectName("SendButton");
    connect(sendButton, &QPushButton::clicked, this, &ClientWidget::sendMessage);

    // 创建连接状态标签，并设置对象名称
    connectionStatusLabel = new QLabel("未连接", this);
    connectionStatusLabel->setObjectName("ConnectionStatusLabel");

    // 创建垂直布局管理器
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建水平布局管理器用于放置服务器地址输入框和端口输入框
    QHBoxLayout *serverLayout = new QHBoxLayout;
    serverLayout->addWidget(serverAddressLineEdit);
    serverLayout->addWidget(serverPortLineEdit);

    // 创建水平布局管理器用于放置连接按钮和断开连接按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(disconnectButton);

    // 将两个水平布局放进去，然后一个个放其他就可以了
    mainLayout->addLayout(serverLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(connectionStatusLabel);
    mainLayout->addWidget(communicationLog);
    mainLayout->addWidget(messageLineEdit);
    mainLayout->addWidget(sendButton);

    // 别名的用处，可以更加方便的设计样式表

    // 创建 TCP 套接字对象
    socket = new QTcpSocket(this);
    // 将套接字的信号与对应的槽函数连接
    //三个信号，一个是成功于服务都安建立连接后触发
    //一个是于服务器断开连接时候触发
    //一个是有新的数据可以读取时候触发
    connect(socket, &QTcpSocket::connected, this, &ClientWidget::connectedToServer);
    connect(socket, &QTcpSocket::disconnected, this, &ClientWidget::disconnectedFromServer);
    connect(socket, &QTcpSocket::readyRead, this, [this]()
            {
                QByteArray data = socket->readAll();
                QString message = QString::fromUtf8(data);

                //刚开始就会发起对战，直接进入游戏
                if (message == "JOIN_CONFIRM")
                {
                    qDebug()<<'I'<<'\n';
                    enterGameMap();
                }
                //如果服务端赢了，就跳出失败
                else if(message == "GAME_OVER_my")
                {
                    levelonepa->showFailureDialog();
                    levelonepa->close();
                }
                //如果服务端输了，就跳出成功
                else if(message == "GAME_OVER_you")
                {
                    levelonepa->showSuccessDialog();
                    levelonepa->close();
                }
                //这里就作为普通的聊天内容读取
                else
                {
                        QString message = QString::fromUtf8(data);
                        displayMessage("服务端: " + message);
                }
            });
    // 创建发送对战请求按钮
    QPushButton *startGameButton = new QPushButton("开始对战", this);
    startGameButton->setObjectName("StartGameButton"); // 设置对象名称以在样式表中使用
    connect(startGameButton, &QPushButton::clicked, this, &ClientWidget::sendJoinRequest);
    startGameButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent; border: 2px solid white; border-radius: 10px; "
        "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
        "}"
        "QPushButton:hover {"
        "color: lightgray;"  // 悬停时的颜色变化
        "}");
    mainLayout->addWidget(startGameButton);

    setStyleSheet(
        "QLineEdit, QPushButton, QTextEdit, QLabel { background-color: rgba(0, 0, 0, 200); color: white; "
        "border: 1px solid #ccc; border-radius: 4px; padding: 5px; }"
        "QPushButton { font-size: 16px; padding: 10px 20px; margin: 4px 2px; cursor: pointer; border-radius: 4px; }"
        "QPushButton:hover { background-color: #0066CC; }"
        "QTextEdit, QLineEdit { min-height: 50px; }"
        "#CommunicationLog { background-color: rgba(0, 0, 0, 200); color: white; border: 1px solid #ccc; "
        "border-radius: 4px; padding: 5px; font-size: 14px; }"
        "#ConnectionStatusLabel { font-size: 18px; color: white; }"
        );
}

//断开与服务器的连接
ClientWidget::~ClientWidget()
{
    socket->disconnectFromHost();
    socket->waitForDisconnected();
    //实现一个优雅的断开函数
    //请求与远程主机的断开连接
    //调用 disconnectFromHost() 后，套接字的状态会变为 QAbstractSocket::ClosingState，表示正在关闭连接。连接断开后，套接字的状态会变为 QAbstractSocket::UnconnectedState。
    //waitForDisconnected() 是 QTcpSocket 类的另一个成员函数。它是一个阻塞函数，等待直到套接字的状态变为 QAbstractSocket::UnconnectedState 或超时。
    /*
     * waitForDisconnected() 是一个阻塞调用，这意味着它会暂停程序的执行，直到满足以下条件之一：
套接字成功断开连接。
超时时间达到。
     * */
}

//然后就连接到指定的主机和端口号
void ClientWidget::connectToServer()
{
    // 获取服务器地址和端口号
    QString hostAddress = serverAddressLineEdit->text();
    quint16 port = serverPortLineEdit->text().toUShort();
    qDebug() << "Attempting to connect to server at address:" << hostAddress << "port:" << port;
    // 尝试连接到服务器
    socket->connectToHost(hostAddress, port);
    //底层的TCP协议会进行三次握手来建立连接
}

//断开与服务器的连接
void ClientWidget::disconnectFromServer()
{
    socket->disconnectFromHost();
    //这里则是底层TCP协议会进行四次挥手来优雅的关闭连接
}

//发送消息到服务器
void ClientWidget::sendMessage()
{
    QString message = messageLineEdit->text();
    if (!message.isEmpty())
    {
        QByteArray data = message.toUtf8();
        //将消息数据写入到与服务器连接的socket
        socket->write(data);
        //在这里底层的TCP协议会进行数据分段，每一个小段落会被加上TCP头部信息，包括序列号，确认好，校验和等
        //数据放入发送被缓存区，TCP/IP协议栈会从这个缓存区读取数据并发送
        //接着发送缓存区的数据
        //接收方会接收这些TCP段，并发送ACK（确认）报文，每个段在被接受方确认接收后，缓存区中会移除相应的数据
        //如果发送方一定时间内没有收到ACK，那么就会重新传未确认的段
        //TCP具有流量控制和拥塞控制机制，可以确保网络不会过载，并且接收方可以处理所有接收到的数据
        messageLineEdit->clear();
        displayMessage("客户端: " + message);
    }
}

//连接到放服务器后，禁用一些按钮，并显示发送的消息
void ClientWidget::connectedToServer()
{
    connectButton->setEnabled(false);
    disconnectButton->setEnabled(true);
    sendButton->setEnabled(true);
    connectionStatusLabel->setText("已连接");
}

//断开与服务器的连接
void ClientWidget::disconnectedFromServer()
{
    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
    sendButton->setEnabled(false);
    connectionStatusLabel->setText("未连接");
}


//在消息日志中追加信息
void ClientWidget::displayMessage(const QString &message)
{
    communicationLog->append(message);
}

//展示错误消息
void ClientWidget::displayError(const QString &error)
{
    communicationLog->append("错误: " + error);
}

//事件过滤器
//主要就是对象和事件，确定对象是，然后事件也是，接着把事件转换为键盘事件，进行处理
//不然直接处理基类
bool ClientWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == messageLineEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            sendMessage();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}


// 获取局域网IP地址的函数
//一个最重要的一部分
/*
获取客户端所在局域网中的 IP 地址。
遍历系统中所有的网络接口，找到第一个 IPv4 地址并返回。
如果没有找到合适的 IP 地址，则返回默认地址 "127.0.0.1"，即本地回环地址。
*/
QString ClientWidget::getLocalIPAddress()
{
    const QList<QHostAddress> &addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : addresses)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost)
        {
            return address.toString();
        }
    }
    return "127.0.0.1"; // 如果没有找到局域网IP地址，则返回localhost地址
}
/*
 当套接字连接到本地回环地址时，数据将在计算机内部传递，而不会经过网络接口发送到网络上。
*/
/*
 * 当套接字连接到网络接口的IP地址时，数据将通过网络接口发送到网络上的其他设备。
*/



void ClientWidget::sendJoinRequest()
{
    QByteArray joinRequestData = "JOIN_REQUEST";
    socket->write(joinRequestData);
}

void ClientWidget::readyReadHandler()
{

}

void ClientWidget::enterGameMap()
{
    levelonepa=new LevelOnepa();
    levelonepa->setWindowTitle("客户端对战");
    //levelonepa->mainLayout->setContentsMargins(10, 0, 800, 0);
    // levelonepa->setMaximumSize(2000,1200);
    // levelonepa->setMinimumSize(2000,1200);
    connect(levelonepa,&LevelOnepa::gameEnded,this,&ClientWidget::checkGameOver);
    levelonepa->show();
    this->hide();
    // levelonepa->resize(1600, 1200);
    // levelonepa->setWindowState(Qt::WindowFullScreen);
    // // 加载背景图片
    // QImage backgroundImage(":/new/images/94dbf2c25da5df4c01031cf128ef0ab.png");

    // // 缩放背景图片以适应窗口大小（忽略宽高比）
    // backgroundImage = backgroundImage.scaled(levelonepa->size(), Qt::IgnoreAspectRatio);

    // // 创建调色板并设置背景
    // QPalette palette;
    // palette.setBrush(levelonepa->backgroundRole(), QBrush(backgroundImage));
    // levelonepa->setPalette(palette);

    // 设置布局边距
}

void ClientWidget::checkGameOver()
{
    if(levelonepa->victory==1)
    {
        QByteArray GameOver = "Game_Over_my";
        socket->write(GameOver);
        levelonepa->close();
    }
    else
    {
        QByteArray GameOver = "Game_Over_you";
        socket->write(GameOver);
        levelonepa->close();
    }
}
