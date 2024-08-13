#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include<QLabel>
#include<QPlainTextEdit>

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();
    void on_darkModeSwitch_changed(int state);

signals:
    void returnToMainPage(); // 发送返回主页面信号

private slots:
    void on_ReturnButton_clicked();
private:
    QPushButton *returnButton; // 返回按钮
    QVBoxLayout *vlayout; // 用于放置返回按钮的垂直布局
    QLabel *backgroundLabel; // 背景图片标签
    QPixmap backgroundPixmap; // 背景图片
    QPlainTextEdit *helpText;
};

#endif // SETTINGSPAGE_H
