#include "CustomHintDialog.h"
#include <QPalette>

CustomHintDialog::CustomHintDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint); // 无边框对话框
    setupUI();
}

CustomHintDialog::~CustomHintDialog()
{
}

void CustomHintDialog::setupUI()
{
    setWindowTitle("提示");
    setFixedSize(600, 300);

    // 设置背景颜色和透明度
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为90%
    setGraphicsEffect(opacityEffect);

    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    QFont font("Microsoft YaHei", 14, QFont::Bold); // 使用微软雅黑字体，设置为18号加粗
    label->setFont(font);
    label->setStyleSheet("color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);

    closeButton = new QPushButton("关闭", this);
    closeButton->setStyleSheet("QPushButton {"
                               "background-color: #007bff; /* 蓝色 */"
                               "border: none; border-radius: 10px; "
                               "font-family: 'Microsoft YaHei'; font-size: 16px; color: white; padding: 10px 20px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #0056b3; /* 深蓝色 */"
                               "}");

    connect(closeButton, &QPushButton::clicked, this, &CustomHintDialog::reject);

    layout->addWidget(closeButton);
    layout->setAlignment(closeButton, Qt::AlignCenter);
    layout->setSpacing(30);
    setLayout(layout);

    // 启用鼠标跟踪
    setMouseTracking(true);
}

void CustomHintDialog::setResultText(const QString &text)
{
    label->setText(text);
}

// 重写鼠标按下事件，以支持对话框的移动
void CustomHintDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

// 重写鼠标移动事件，以支持对话框的移动
void CustomHintDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}
