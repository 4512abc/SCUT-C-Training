#include "UsernameFind.h"
#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QHBoxLayout>

UsernameFind::UsernameFind(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("找回密码"));
    setFixedSize(600, 300);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框对话框

    // 设置背景颜色和透明度
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);

    // 标题
    titleLabel = new QLabel(tr("请输入您的用户名:"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");
    // 用户名输入框
    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 28px; color: black;"); // 修改字体颜色为黑色

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
    cancelButton=new QPushButton(tr("取消"),this);
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
    mainLayout->addWidget(usernameLineEdit);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    connect(okButton, &QPushButton::clicked, this, &UsernameFind::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &UsernameFind::onCancelClicked);
}

QString UsernameFind::getUsername()
{
    if (exec() == QDialog::Accepted) {
        return usernameLineEdit->text();
    } else {
        return QString();
    }
}

void UsernameFind::onOkClicked()
{
    accept();
}

void UsernameFind::onCancelClicked()
{
    reject();
}
