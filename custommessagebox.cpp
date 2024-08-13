#include "custommessagebox.h"
#include <QVBoxLayout>
#include <QApplication>

CustomMessageBox::CustomMessageBox(const QString &message, QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint) // 无边框设置
{
    setWindowTitle("警告");
    setFixedSize(600, 300);

    // 设置背景颜色和透明度
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);

    // 设置文本样式
    messageLabel = new QLabel(message, this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 30px; color: white;");

    // 设置按钮样式
    okButton = new QPushButton("确定", this);
    okButton->setStyleSheet("QPushButton {"
                            "background-color: #FF0000; border: none; border-radius: 10px; "
                            "font-family: 'Microsoft YaHei'; font-size: 25px; color: white;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #CC0000;"
                            "}");
    okButton->setFixedSize(150, 50);

    connect(okButton, &QPushButton::clicked, this, &CustomMessageBox::onOkClicked);

    // 布局设置
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(messageLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

void CustomMessageBox::onOkClicked()
{
    accept();
}
