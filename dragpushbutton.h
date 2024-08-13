#ifndef DRAGPUSHBUTTON_H
#define DRAGPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>

class YDragButton : public QPushButton
{
    Q_OBJECT

public:
    explicit YDragButton(QWidget *parent = 0);
    ~YDragButton();
    void setMargin(int left, int top, int right, int bottom);

signals:
    void clicked();
    void doubleClicked();
    void toggled(bool isToggled);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QString text;             // 显示文字
    bool isPressed;           // 鼠标是否按下
    QPoint lastPoint;         // 鼠标按下最后坐标
    QString type;             // 图片末尾类型
    QString imgPath;          // 背景图片路径
    QString imgName;          // 背景图片名称
    int iconWidth = 0;        // 图片宽度
    int iconHeight = 0;       // 图片高度
    bool isMoved;             // 是否正在移动中
    int x_left_distance;      // 可移动的距离左侧的距离
    int y_top_distance;       // 可移动的距离上面的距离
    int x_right_distancce;    // 可移动的距离右侧的距离
    int y_bottom_distance;    // 可移动的距离上底部的距离
    bool isChecked = false;   // 按钮的选中状态
};

#endif
