#include "logindialog.h"

#include <QVBoxLayout>
#include <QDateTime>
#include <QGuiApplication>
#include <QScreen>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QFileDialog>
#include<QKeyEvent>
#include<QMenu>
#include<QApplication>
#include<QDrag>
#include<QMimeData>
#include "custommessagebox.h"
#include "UsernameFind.h"
#include "PasswordFind.h"

LoginDialog::LoginDialog(QWidget *parent) : QWidget(parent)
{
    //打开或创建 vvv.txt 文件。如果文件为空，则写入默认内容 放课后TeaTime ac。
    QFile file("./vvv.txt");
    QFileInfo fileInfo(file);
    if (fileInfo.size() == 0)
    {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out <<"放课后TeaTime ac";
            file.close();
        }
        else{}
    }
    else{}

    //初始化成员变量 _password 和 _username，并设置窗口标题为"登录"。
    _password="";
    _username="";
    setWindowTitle("登录");
    // 创建头像标签
    avatarLabel = new QLabel(this);
    avatarLabel->setFixedSize(150, 150);
    avatarLabel->setAlignment(Qt::AlignCenter);

    // 在用户名和密码文本框之前创建一个占位符标签，用于调整布局
    QLabel *placeholderLabel = new QLabel(this);
    placeholderLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);

    //定义一个方便css操作的对象
    usernameEdit->setObjectName("usernameEdit");
    passwordEdit->setObjectName("passwordEdit");
    passwordEdit->setEchoMode(QLineEdit::Password);

    //样式表
    QString buttonStyle =(   "QPushButton {"
                           "background-color: transparent; border: 2px solid white; border-radius: 10px; "
                           "font-family: 'Microsoft YaHei'; font-size: 35px; font-weight: bold; color: white;"
                           "}"
                           "QPushButton:hover {"
                           "color: lightgray;"  // 悬停时的颜色变化
                           "}");

    //创建登录按钮 loginButton
    loginButton = new QPushButton("登录", this);
    loginButton->setStyleSheet(buttonStyle);
    loginButton->setIcon(QIcon(":/new/images/dl.png"));
    loginButton->setIconSize(QSize(40, 40));

    //创建返回按钮 returnButton
    returnButton = new QPushButton("返回", this);
    returnButton->setStyleSheet(buttonStyle);
    returnButton->setIcon(QIcon(":/new/images/return.png"));
    returnButton->setIconSize(QSize(40, 40));

    //创建注册按钮 registerButton
    registerButton = new QPushButton("注册", this);
    registerButton->setStyleSheet(buttonStyle);
    registerButton->setIcon(QIcon(":/new/images/zc.png"));
    registerButton->setIconSize(QSize(32, 32));

    //创建忘记密码按钮 forgotPasswordButton
    forgotPasswordButton = new QPushButton("忘记密码", this);
    forgotPasswordButton->setStyleSheet(buttonStyle);
    forgotPasswordButton->setIcon(QIcon(":/new/images/mm.png"));
    forgotPasswordButton->setIconSize(QSize(30, 30));

    //创建上传头像按钮 uploadAvatarButton
    uploadAvatarButton = new QPushButton("上传头像", this);
    uploadAvatarButton->setStyleSheet(buttonStyle);
    uploadAvatarButton->setIcon(QIcon(":/new/images/sctx.png"));
    uploadAvatarButton->setIconSize(QSize(40, 40));

    connect(uploadAvatarButton, &QPushButton::clicked, this, &LoginDialog::uploadAvatar);

    QFont buttonFont("Microsoft YaHei", 14);
    loginButton->setFont(buttonFont);
    returnButton->setFont(buttonFont);
    registerButton->setFont(buttonFont);
    forgotPasswordButton->setFont(buttonFont);
    uploadAvatarButton->setFont(buttonFont);

    //创建主布局 mainLayout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(returnButton, 0, Qt::AlignTop | Qt::AlignLeft);
    mainLayout->addWidget(avatarLabel);
    mainLayout->addWidget(placeholderLabel);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addWidget(passwordEdit);

    //放入按钮的布局
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(25);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(forgotPasswordButton);
    buttonLayout->addWidget(uploadAvatarButton);
    buttonLayout->setAlignment(Qt::AlignCenter); // 按钮居中显示

    //设置边界
    buttonLayout->setContentsMargins(0,0,900,1100);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(0, 0, 0, 700); // 将整体向上移动100像素
    //设置为主布局
    setLayout(mainLayout);

    //获取屏幕信息，计算窗口的初始位置和大小，并设置窗口的几何形状，使其在屏幕中央显示。
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    int dialogWidth = 400;
    int dialogHeight = 400;
    int x = (screenWidth - dialogWidth) / 2;
    int y = (screenHeight - dialogHeight) / 2 - 100;
    setGeometry(x, y, dialogWidth, dialogHeight);

    //设置窗口的不透明度为0.9，使窗口半透明。
    setWindowOpacity(0.9);

    //将按钮的点击信号连接到相应的槽函数。
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::attemptRegister);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &LoginDialog::forgotPassword);
    connect(returnButton, &QPushButton::clicked, this, &LoginDialog::returnToMainPage);

    //设置用户名和密码输入框的样式
    setStyleSheet
        (
            "#usernameEdit, #passwordEdit {"
            "   border: 5px solid #ccc;"
            "   border-radius: 5px;"
            "   padding: 5px;"
            "   font-size: 18px;"
            "}"
            "#usernameEdit:focus, #passwordEdit:focus {"
            "   border-color: #007bff;"
            "}"
            );

    //创建系统托盘图标，并设置用户名和密码输入框的占位文本和固定宽度。
    createTrayIcon();
    usernameEdit->setPlaceholderText(tr("请输入用户名"));
    passwordEdit->setPlaceholderText(tr("请输入密码"));
    passwordEdit->setFixedWidth(700); // 设置输入框的固定宽度
    usernameEdit->setFixedWidth(700); // 设置输入框的固定宽度
}

//析构函数，销毁托盘图标
LoginDialog::~LoginDialog()
{
    delete trayIcon;
}

//attemptLogin 函数：尝试登录，读取 vvv.txt 文件，检查用户名和密码是否匹配，如果匹配则登录成功，否则提示登录失败。
void LoginDialog::attemptLogin()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    QFile file("./vvv.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(' ');
        if (parts.size() == 2 && parts[0] == username)
        {
            if (parts[1] == password)
            {
                QString loginTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                emit loggedIn(username, loginTime);
                _username = username;
                _password = password;
                CustomMessageBox messageBox("登录成功。");
                messageBox.exec();
                usernameEdit->clear();
                passwordEdit->clear();
                return;
            }
            else
            {
                CustomMessageBox messageBox("密码错误，请重试。");
                messageBox.exec();
                usernameEdit->clear();
                passwordEdit->clear();
                return;
            }
        }
    }
    file.close();
    CustomMessageBox messageBox("用户名未注册，请先注册。");
    messageBox.exec();
    usernameEdit->clear();
    passwordEdit->clear();
}


//尝试注册的槽函数
void LoginDialog::attemptRegister()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    //检查用户名和密码是否为空
    if (username.isEmpty() || password.isEmpty())
    {
        CustomMessageBox messageBox("用户名和密码不能为空。");
        messageBox.exec();
        return;
    }

    // 检查用户名是否已注册
    QFile file("./vvv.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(' ');
        if (parts.size() == 2 && parts[0] == username)
        {
            file.close();
            CustomMessageBox messageBox("用户名已经被注册，请选择其他用户名。");
            messageBox.exec();
            return;
        }
    }
    file.close();

    // 以追加模式打开文件
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);
    out <<'\n'<< username << " " << password << "\n";
    file.close();

    _username = username;
    _password = password;
    CustomMessageBox messageBox("注册成功，请用户登录。");
    messageBox.exec();
    usernameEdit->clear();
    passwordEdit->clear();
}

//展示错误聊天框
void LoginDialog::showError(const QString &errorMessage)
{
    QMessageBox::critical(this, "错误", errorMessage);
}

//展示成功聊天框
void LoginDialog::showSuccess(const QString &message)
{
    QMessageBox::information(this, "提示", message);
}


//通过输入用户名来查找密码，并显示找回的密码。
void LoginDialog::forgotPassword()
{
    bool ok=1;
    QString username = UsernameFind(this).getUsername();
    if (ok && !username.isEmpty())
    {
        QString newPassword = PasswordFind(this).getNewPassword();
        if (ok && !newPassword.isEmpty())
        {
            QFile file("./vvv.txt");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                return;
            }

            QStringList fileContent;
            bool userFound = false;
            QTextStream in(&file);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                QStringList parts = line.split(' ');
                if (parts.size() == 2 && parts[0] == username)
                {
                    fileContent.append(username + " " + newPassword);
                    userFound = true;
                }
                else
                {
                    fileContent.append(line);
                }
            }
            file.close();
            if (userFound)
            {
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                {
                    showError("无法打开用户数据文件。");
                    return;
                }
                QTextStream out(&file);
                for (const QString &line : fileContent)
                {
                    out << line << "\n";
                }
                file.close();

                _password = newPassword;
                CustomMessageBox messageBox("密码重置成功，请使用新密码登录。");
                messageBox.exec();

            }
            else
            {
                CustomMessageBox messageBox("用户名未注册。");
                messageBox.exec();

            }
        }
    }
}

void LoginDialog::uploadAvatar()
{
    // 打开文件对话框选择图片
    imagePath = QFileDialog::getOpenFileName(this, "选择头像", QDir::homePath(), "Images (*.png *.jpg *.jpeg)");
    if (!imagePath.isEmpty())
    {
        // 加载图片并显示在对话框上
        QPixmap avatarPixmap(imagePath);
        if (!avatarPixmap.isNull())
        {
            // 截取图片
            QPixmap roundedAvatar = avatarPixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            roundedAvatar.setMask(roundedAvatar.createHeuristicMask());
            // 在对话框上显示截取后的图片
            avatarLabel->setPixmap(roundedAvatar);
            avatarLabel->move(200,80);
        }
    }
}

//returnToMainPage 函数：关闭当前窗口，返回主页面。
void LoginDialog::on_ReturnButton_clicked()
{
    emit returnToMainPage();
}

//重写键盘处理函数
void LoginDialog::keyPressEvent(QKeyEvent *event)
{
    // 检查按下的键是否是回车键
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // 检查用户名和密码是否都非空
        if (!usernameEdit->text().isEmpty() && !passwordEdit->text().isEmpty())
        {
            // 调用尝试登录函数
            attemptLogin();
        }
    }
    else
    {
        // 其他情况交给父类处理
        QWidget::keyPressEvent(event);
    }
}


void LoginDialog::attemptlogin()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    // 打开文件
    QFile file("./vvv.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    bool isRegistered = false;

    // 读取文件内容并检查用户名和密码
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(' ');
        if (parts.size() == 2 && parts[0] == username)
        {
            isRegistered = true;
            if (parts[1] == password)
            {
                QString loginTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                emit loggedIn(username, loginTime);
                _username = username;
                _password = password;
                CustomMessageBox messageBox("登录成功。");
                messageBox.exec();
                usernameEdit->clear();
                passwordEdit->clear();
                file.close();
                return;
            }
            else
            {
                CustomMessageBox messageBox("密码错误，请重试。");
                messageBox.exec();
                usernameEdit->clear();
                passwordEdit->clear();
                file.close();
                return;
            }
        }
    }
    file.close();

    if (!isRegistered)
    {
        CustomMessageBox messageBox("用户名未注册，请先注册。");
        messageBox.exec();
    }
    usernameEdit->clear();
    passwordEdit->clear();
}

//创建系统托盘图标，并设置其右键菜单，包含恢复和退出选项。
void LoginDialog::createTrayIcon()
{
    // 创建托盘图标
    trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/new/images/gdyg2.png"); // 选择一个合适的图标路径
    trayIcon->setIcon(icon);

    // 创建托盘菜单
    QMenu *trayMenu = new QMenu(this);
    minimizeAction = new QAction("最小化", this);
    restoreAction = new QAction("还原", this);
    quitAction = new QAction("退出", this);
    /*
     创建三个 QAction 对象，分别用于“最小化”、“还原”和“退出”操作。每个动作都有一个文本标签，并且它们都属于当前的 LoginDialog 对象。
    */
    connect(minimizeAction, &QAction::triggered, this, &LoginDialog::hide);
    connect(restoreAction, &QAction::triggered, this, &LoginDialog::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    trayMenu->addAction(minimizeAction);
    trayMenu->addAction(restoreAction);
    trayMenu->addAction(quitAction);
    // 设置托盘菜单
    trayIcon->setContextMenu(trayMenu);
    // 显示托盘图标
    trayIcon->show();
}

// 新添加的函数
QString LoginDialog::getUsername() const
{
    return _username;
}

QString LoginDialog::getPassword() const
{
    return _password;
}

QString LoginDialog::getAvatarPath() const
{
    return imagePath;
}











