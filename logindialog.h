// logindialog.h
#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include "config.h"
#include <QWidget>
#include "dragpushbutton.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include<QSystemTrayIcon>

class LoginDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    void forgotPassword();
    QString getUsername() const;
    QString getPassword() const;
    QString getAvatarPath() const;
    void saveUser(const QString &username, const QString &password);

signals:
    void loggedIn(const QString &username, const QString &loginTime);
    void returnToMainPage();

private slots:
    void attemptLogin();
    void attemptRegister();
    void showError(const QString &errorMessage);
    void showSuccess(const QString &message);
    void on_ReturnButton_clicked();
    void uploadAvatar();
    void keyPressEvent(QKeyEvent *event); // 新增的键盘事件处理函数声明
    void attemptlogin();

private:
    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *returnButton;
    QPushButton *registerButton;
    QPushButton *forgotPasswordButton;
    QCheckBox *rememberPasswordCheckbox;
    QCheckBox *autoLoginCheckbox;
    QPushButton *uploadAvatarButton;
    QLabel *avatarLabel; // 添加头像标签

    // 新增保存注册信息的数据结构
    struct UserInfo
    {
        QString username;
        QString password;
    };
    bool  customLayoutEnabled;
    QSystemTrayIcon *trayIcon;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QPushButton *customLayoutButton;
    void createTrayIcon();
    QString imagePath;
    QString _password;
    QString _username;
    QList<UserInfo> registeredUsers; // 保存注册用户信息的列表
};

#endif // LOGINDIALOG_H
