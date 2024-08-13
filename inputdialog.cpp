#include "inputdialog.h"
#include <QGraphicsOpacityEffect>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include<QTimer>
InputDialog::InputDialog(QWidget *parent)
    : QInputDialog(parent)
{
    setWindowTitle(tr("编辑地图"));
    setLabelText(tr("请输入数字："));
    setInputMode(QInputDialog::TextInput);
    setFixedSize(600, 300);  // 调整对话框大小
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框对话框

    // 设置背景颜色和透明度
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);

    // 设置输入框文本样式
    QFont font("Microsoft YaHei");
    font.setPixelSize(24);
    setFont(font);
    setStyleSheet("QLineEdit {"
                  "    background-color: #303030; /* 输入框背景颜色 */"
                  "    border: none; /* 去掉边框 */"
                  "    border-radius: 10px; /* 圆角 */"
                  "    padding: 10px; /* 内边距 */"
                  "    color: #FFFFFF; /* 文字颜色 */"
                  "}"
                  "QLabel {"
                  "    color: #FFFFFF; /* 标签文字颜色 */"
                  "}");

    // 修改按钮样式
    // 定时器延迟执行查找按钮和设置样式
    QTimer::singleShot(0, this, [this]() {
        QList<QPushButton *> buttons = findChildren<QPushButton *>();
        if (buttons.size() >= 2) {
            buttons[0]->setStyleSheet("QPushButton {"
                                      "    background-color: #FF0000; /* 确认按钮红色 */"
                                      "    border: none; /* 去掉边框 */"
                                      "    border-radius: 10px; /* 圆角 */"
                                      "    padding: 10px; /* 内边距 */"
                                      "    font-family: 'Microsoft YaHei';"
                                      "    font-size: 25px;"
                                      "    color: white;"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: #CC0000;"
                                      "}");
            buttons[1]->setStyleSheet("QPushButton {"
                                      "    background-color: #E0E0E0; /* 取消按钮浅白色 */"
                                      "    border: none; /* 去掉边框 */"
                                      "    border-radius: 10px; /* 圆角 */"
                                      "    padding: 10px; /* 内边距 */"
                                      "    font-family: 'Microsoft YaHei';"
                                      "    font-size: 25px;"
                                      "    color: black;"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: #C0C0C0;"
                                      "}");
        }
    });
}

QString InputDialog::getTextValue()
{
    if (exec() == Accepted)
    {
        return textValue();
    }
    return QString();
}
