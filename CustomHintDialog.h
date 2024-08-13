#ifndef CUSTOMHINTDIALOG_H
#define CUSTOMHINTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>

class CustomHintDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomHintDialog(QWidget *parent = nullptr);
    ~CustomHintDialog();
    //设置文本进行提示
    void setResultText(const QString &text);

protected:
    //实现窗口自拖动
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setupUI();
    QLabel *label;
    QPushButton *closeButton;
    QPoint m_dragPosition; // 记录鼠标拖动的位置
    //这里要通过相对距离计算

};

#endif // CUSTOMHINTDIALOG_H
