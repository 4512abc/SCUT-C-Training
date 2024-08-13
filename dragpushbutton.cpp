#pragma execution_character_set("utf-8") // 设置执行字符集为 UTF-8
#include "dragpushbutton.h"
#include<QEvent>
#include<QMouseEvent>

// YDragButton 的构造函数
YDragButton::YDragButton(QWidget *parent) : QPushButton(parent)
{
    // 初始化成员变量
    isPressed = false;
    // 鼠标是否被按下
    isMoved = false;
    // 按钮是否被拖动
    lastPoint = QPoint();
    // 鼠标按下时的最后一个点
    x_left_distance = 0;
    // 左边距
    x_right_distancce = 0;
    // 右边距
    y_top_distance = 0;
    // 上边距
    y_bottom_distance = 0;
    // 下边距
    this->installEventFilter(this);
    // 安装事件过滤器
}

// YDragButton 的析构函数
YDragButton::~YDragButton()
{
    // 默认析构函数为空
}

// 设置按钮移动的边距
void YDragButton::setMargin(int left, int top, int right, int bottom)
{
    x_left_distance = left;
    // 设置左边距
    y_top_distance = top;
    // 设置上边距
    x_right_distancce = right;
    // 设置右边距
    y_bottom_distance = bottom;
    // 设置下边距
}

// 事件过滤器，用于处理鼠标事件
bool YDragButton::eventFilter(QObject *watched, QEvent *event)
{
    // 将事件转换为鼠标事件
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    switch(event->type())
    {
    case QEvent::MouseButtonPress:
        // 鼠标按下事件
        {
            // 检查是否是右键按下
            if (mouseEvent->button() == Qt::RightButton)
            {
                lastPoint = mouseEvent->pos();
                // 记录按下时的点
                isPressed = true;
                // 标记鼠标被按下
            }
            break;
        }
    case QEvent::MouseMove: // 鼠标移动事件
    {
        if (isPressed)
        {
            int dx = mouseEvent->pos().x() - lastPoint.x();
            // 计算鼠标在 X 方向的移动距离
            int dy = mouseEvent->pos().y() - lastPoint.y();
            // 计算鼠标在 Y 方向的移动距离
            int x1 = this->x() + dx;
            // 计算按钮的新 X 坐标
            int y1 = this->y() + dy;
            // 计算按钮的新 Y 坐标
            int right_distance = this->parentWidget()->width() - 2 * x_right_distancce - this->width();
            // 计算按钮在父窗口中的最大右边距
            int bottom_distance = this->parentWidget()->height() - 2 * y_bottom_distance - this->height();
            // 计算按钮在父窗口中的最大下边距
            // 检查新位置是否在允许范围内
            if (x1 > x_left_distance && x1 < right_distance && y1 > y_top_distance && y1 < bottom_distance)
                this->move(x1, y1);
            // 移动按钮到新位置
            isMoved = true;
            // 标记按钮被拖动
        }
        break;
    }
    case QEvent::MouseButtonRelease:
        // 鼠标释放事件
        {
            if(isMoved != true)
            {
                // 如果按钮没有被拖动
                emit clicked();
                // 触发 clicked 信号
                emit toggled(!isChecked);
                // 触发 toggled 信号
                isChecked = !isChecked;
                // 切换按钮的选中状态
            }
            else
            {
                isMoved = false;
                // 重置拖动标记
            }
            isPressed = false;
            // 重置按下标记
            break;
        }
    case QEvent::MouseButtonDblClick:
        // 鼠标双击事件
        emit doubleClicked();
        // 触发 doubleClicked 信号
        break;
    default:
        break;
    }
    return QWidget::eventFilter(watched, event);
    // 调用基类的事件过滤器
}
