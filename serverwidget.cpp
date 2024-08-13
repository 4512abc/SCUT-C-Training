#include "serverwidget.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QNetworkInterface>
#include <QProgressBar>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include "MsgBox.h"

ServerWidget::ServerWidget(QWidget *parent) : QWidget(parent)
{
    setWindowIcon(QIcon(":/new/images/hx.png"));
    resize(1256, 707);

    // 设置透明背景
    QImage background(":/new/images/94dbf2c25da5df4c01031cf128ef0ab.png");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    background = background.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio);
    palette.setBrush(this->backgroundRole(), QBrush(background));
    this->setPalette(palette);

    // 获取本地IP地址
    QString hostAddress = getLocalIPAddress();

    // 创建主机地址输入框，并设置为只读
    hostLineEdit = new QLineEdit(hostAddress, this);
    hostLineEdit->setReadOnly(true);
    hostLineEdit->setPlaceholderText("主机地址");

    // 创建端口号输入框，并设置为只读
    portLineEdit = new QLineEdit("1200", this);
    portLineEdit->setReadOnly(true);
    portLineEdit->setPlaceholderText("端口号");

    // 创建启动服务器按钮，并连接到槽函数
    startButton = new QPushButton("启动服务器", this);
    startButton->setObjectName("StartButton");
    connect(startButton, &QPushButton::clicked, this, &ServerWidget::startServer);

    // 创建终止服务器按钮，并连接到槽函数
    stopButton = new QPushButton("终止服务器", this);
    stopButton->setEnabled(false);
    stopButton->setObjectName("StopButton");
    connect(stopButton, &QPushButton::clicked, this, &ServerWidget::stopServer);

    // 创建选择文件按钮，并连接到槽函数
    fileTransferButton = new QPushButton("选择文件", this);
    fileTransferButton->setEnabled(false);
    connect(fileTransferButton, &QPushButton::clicked, this, &ServerWidget::selectFile);

    // 创建进度条
    progressBar = new QProgressBar(this);
    progressBar->setObjectName("ProgressBar");
    progressBar->setValue(0);

    // 创建通信日志文本框，并设置为只读
    communicationLog = new QTextEdit(this);
    communicationLog->setObjectName("CommunicationLog");
    communicationLog->setReadOnly(true);
    communicationLog->setStyleSheet("background-color: rgba(0, 0, 0, 200); color: white; border: 1px solid #ccc; border-radius: 4px; padding: 5px;");

    // 创建消息输入框
    messageLineEdit = new QLineEdit(this);
    messageLineEdit->setPlaceholderText("随心所欲吧...");
    messageLineEdit->setMinimumHeight(50);
    messageLineEdit->installEventFilter(this);
    messageLineEdit->setStyleSheet("background-color: rgba(0, 0, 0, 200); color: white; border: 1px solid #ccc; border-radius: 4px; padding: 5px;");

    // 创建发送按钮，并连接到槽函数
    sendButton = new QPushButton("发送", this);
    sendButton->setEnabled(false);
    sendButton->setObjectName("SendButton");
    connect(sendButton,&QPushButton::clicked,this,&ServerWidget::sendMessage);
    // 创建服务器状态标签
    serverStatusLabel = new QLabel("服务端还未启动", this);
    serverStatusLabel->setObjectName("ServerStatusLabel");
    serverStatusLabel->setStyleSheet("font-size: 18px; color: white;");

    // 创建布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *hostLayout = new QHBoxLayout;
    hostLayout->addWidget(hostLineEdit);
    hostLayout->addWidget(portLineEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);

    QHBoxLayout *fileTransferLayout = new QHBoxLayout;
    fileTransferLayout->addWidget(fileTransferButton);
    fileTransferLayout->addWidget(progressBar);

    mainLayout->addLayout(hostLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addWidget(communicationLog);
    mainLayout->addWidget(messageLineEdit);
    mainLayout->addWidget(sendButton);
    mainLayout->addLayout(fileTransferLayout);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    setStyleSheet(
        "QLineEdit, QPushButton, QTextEdit, QLabel { background-color: rgba(0, 0, 0, 200); color: white; "
        "border: 1px solid #ccc; border-radius: 4px; padding: 5px; }"
        "QPushButton { font-size: 16px; padding: 10px 20px; margin: 4px 2px; cursor: pointer; border-radius: 4px; }"
        "QPushButton:hover { background-color: #0066CC; }"
        "QTextEdit, QLineEdit { min-height: 50px; }"
        "#CommunicationLog { background-color: rgba(0, 0, 0, 200); color: white; border: 1px solid #ccc; "
        "border-radius: 4px; padding: 5px; font-size: 14px; }"
        "#ServerStatusLabel { font-size: 18px; color: white; }"
        );

    // 创建服务器对象，并连接信号槽
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerWidget::incomingConnection);

    // 设置窗口标题
    setWindowTitle("服务端");

}


ServerWidget::~ServerWidget()
{
    // 停止服务器
    if (server)
    {
        stopServer();
        delete server;
    }
}

//开始连接到服务器，获取主机号和端口号，连接到指定的主机和端口号
void ServerWidget::startServer()
{
    // 获取主机地址和端口号
    QString hostAddress = hostLineEdit->text();
    qint16 port = portLineEdit->text().toInt();

    // 如果服务器成功启动，则禁用启动按钮，启用终止按钮、发送按钮和选择文件按钮，并更新服务器状态标签
    if (server->listen(QHostAddress(hostAddress), port))
    {
        startButton->setEnabled(false);
        stopButton->setEnabled(true);
        sendButton->setEnabled(true);
        fileTransferButton->setEnabled(true);
        serverStatusLabel->setText("服务端正在运行: " + hostAddress + ":" + QString::number(port));
    }
    else
    {
        // 若无法启动服务器，则在通信日志中记录错误信息
        communicationLog->append("无法启动服务端: " + server->errorString());
    }
}

//停止服务器
void ServerWidget::stopServer()
{
    server->close();
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    sendButton->setEnabled(false);
    fileTransferButton->setEnabled(false);
    serverStatusLabel->setText("服务端还未启动");
    communicationLog->clear();
}

//选择文件
void ServerWidget::selectFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择您的文件进行发送");
    if (!filePath.isEmpty())
    {
        selectedFilePath = filePath;
        QString fileName = QFileInfo(filePath).fileName();
        sendFile(fileName);
    }
}

//发送那个文件
void ServerWidget::sendFile(const QString &fileName)
{
    QFile file(selectedFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        communicationLog->append("无法打开文件: " + file.errorString());
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    // 发送文件名和文件大小
    for (QTcpSocket *clientSocket : clients)
    {
        QDataStream outStream(clientSocket);
        outStream << fileName << fileData;
    }
    displayMessage("服务端: 文件 '" + fileName + "' 已经发送给客户端");
}

//发送信息
void ServerWidget::sendMessage()
{
    QString message = messageLineEdit->text();
    if (!message.isEmpty()) {
        QByteArray data = message.toUtf8();
        for (QTcpSocket *clientSocket : clients)
        {
            clientSocket->write(data);
            displayMessage("服务端: " + message);
        }
        messageLineEdit->clear();
    }
}

//新的客户端连接到
void ServerWidget::incomingConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *clientSocket = server->nextPendingConnection();
        clients.append(clientSocket);
        connect(clientSocket, &QTcpSocket::readyRead, this, &ServerWidget::readyReadHandler);
        connect(clientSocket, &QTcpSocket::disconnected, this, &ServerWidget::disconnectedHandler);

        displayMessage("新客户端连接: "+ clientSocket->peerAddress().toString());
    }
}

//读取数据
void ServerWidget::readyReadHandler()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;

    QByteArray data = clientSocket->readAll();
    QString message = QString::fromUtf8(data);

    if (message == "JOIN_REQUEST")
    {
        handleJoinRequest(clientSocket);
    }
    else if(message == "Game_Over_my")
    {
        levelonepa->showFailureDialog();
        levelonepa->close();
    }
    else if(message == "Game_Over_you")
    {
        levelonepa->showSuccessDialog();
        levelonepa->close();
    }
    else
    {
        QString message = QString::fromUtf8(data);
        displayMessage("客户端: " + message);
    }
}

//断开连接
void ServerWidget::disconnectedHandler()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;
    clients.removeOne(clientSocket);
    clientSocket->deleteLater();
}

//事件过滤器
bool ServerWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == messageLineEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            sendMessage();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

//消息日志中展示信息
void ServerWidget::displayMessage(const QString &message)
{
    communicationLog->append(message);
}

// 获取局域网IP地址的函数
QString ServerWidget::getLocalIPAddress()
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


void ServerWidget::handleJoinRequest(QTcpSocket *clientSocket)
{
    // 创建一个消息框来确认或拒绝加入请求

    if (!clientSocket)
    {
        qDebug() << "clientSocket is nullptr";
        return;
    }

    if (clientSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "clientSocket is not connected";
        return;
    }

    MsgBox msgBox(this);
    // 显示消息框并获取用户的回复
    if (msgBox.exec() == QDialog::Accepted)
    {
        // 接受客户端的请求
        QByteArray joinConfirmData = "JOIN_CONFIRM";
        clientSocket->write(joinConfirmData);
        // 开始游戏或其他必要的操作
        levelonepa=new LevelOnepa();
        levelonepa->setWindowTitle("服务端对战");
        //levelonepa->mainLayout->setContentsMargins(10, 0, 800, 0);
        // levelonepa->setMaximumSize(2000,1200);
        // levelonepa->setMinimumSize(2000,1200);
        connect(levelonepa, &LevelOnepa::gameEnded, this, [this, clientSocket]() {
            QByteArray gameOverData = "GAME_OVER_my";
            qDebug() << "Game ended";
            sendGameOver(clientSocket);
        });
        this->hide();
        levelonepa->show();
        // // 加载背景图片
        // QImage backgroundImage(":/new/images/94dbf2c25da5df4c01031cf128ef0ab.png");
        // backgroundImage = backgroundImage.scaled(levelonepa->size(), Qt::IgnoreAspectRatio);
        // QPalette palette;
        // palette.setBrush(levelonepa->backgroundRole(), QBrush(backgroundImage));
        // levelonepa->setPalette(palette);

        // 设置布局边距
        // 连接游戏结束信号到处理游戏结束的槽函数
    } else
    {
        // 拒绝客户端的请求
        QByteArray joinRejectData = "JOIN_REJECT";
        clientSocket->write(joinRejectData);
        // 可选地，在服务器日志或GUI中记录请求被拒绝的信息
        displayMessage("拒绝了客户端的加入请求。");
    }
}


void ServerWidget::sendGameOver(QTcpSocket *clientSocket)
{
    if(levelonepa->victory==1)
    {
        QByteArray gameOverData = "GAME_OVER_my";
        clientSocket->write(gameOverData);
        qDebug()<<"s"<<'\n';
        levelonepa->close();
    }
    else
    {
        QByteArray gameOverData = "GAME_OVER_you";
        qDebug()<<"s"<<'\n';
        clientSocket->write(gameOverData);
        qDebug()<<"s"<<'\n';
        levelonepa->close();
    }
}
