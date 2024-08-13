#include "ExitDialog.h"
#include <QGraphicsOpacityEffect>

ExitDialog::ExitDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("提示");
    setFixedSize(600, 300);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    // 设置背景颜色和透明度
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);

    messageLabel = new QLabel("您确定要关闭游戏吗?", this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");

    confirmButton = new QPushButton("确认", this);
    cancelButton = new QPushButton("取消", this);

    confirmButton->setStyleSheet("QPushButton {"
                                 "background-color: #FF0000; border: none; border-radius: 10px; "
                                 "font-family: 'Microsoft YaHei'; font-size: 25px; color: white;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color: #CC0000;"
                                 "}");
    confirmButton->setFixedSize(150, 50);

    cancelButton->setStyleSheet("QPushButton {"
                                "background-color: #808080; border: none; border-radius: 10px; "
                                "font-family: 'Microsoft YaHei'; font-size: 25px; color: white;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #696969;"
                                "}");
    cancelButton->setFixedSize(150, 50);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(messageLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addSpacing(30);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(30);

    connect(confirmButton, &QPushButton::clicked, this, &ExitDialog::onConfirmClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ExitDialog::onCancelClicked);
}

void ExitDialog::onConfirmClicked()
{
    accept();
}

void ExitDialog::onCancelClicked()
{
    reject();
}
