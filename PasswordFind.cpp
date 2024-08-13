#include "PasswordFind.h"
#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QHBoxLayout>

PasswordFind::PasswordFind(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("重置密码"));
    setFixedSize(600, 300);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框对话框

    // 设置背景颜色和透明度
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);

    // 标题
    titleLabel = new QLabel(tr("请输入新密码:"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");

    // 密码输入框
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 28px; color: black;"); // 修改字体颜色为黑色

    // 确认按钮
    okButton = new QPushButton(tr("确定"), this);
    okButton->setStyleSheet("QPushButton {"
                            "background-color: #FF0000; border: none; border-radius: 10px; "
                            "font-family: 'Microsoft YaHei'; font-size: 25px; color: white;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #CC0000;"
                            "}");
    okButton->setFixedSize(150, 50);

    // 取消按钮
    cancelButton = new QPushButton(tr("取消"), this);
    cancelButton->setStyleSheet("QPushButton {"
                                "background-color: #808080; border: none; border-radius: 10px; "
                                "font-family: 'Microsoft YaHei'; font-size: 25px; color: white;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #696969;"
                                "}");
    cancelButton->setFixedSize(150, 50);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // 主布局设置
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(passwordLineEdit);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    connect(okButton, &QPushButton::clicked, this, &PasswordFind::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &PasswordFind::onCancelClicked);
}

QString PasswordFind::getNewPassword()
{
    if (exec() == QDialog::Accepted) {
        return passwordLineEdit->text();
    } else {
        return QString();
    }
}

void PasswordFind::onOkClicked()
{
    accept();
}

void PasswordFind::onCancelClicked()
{
    reject();
}
