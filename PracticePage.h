#ifndef PRACTICEPAGE_H
#define PRACTICEPAGE_H
#include "config.h"
#include<QVBoxLayout>
#include<QPushButton>
#include <QWidget>
#include<QStackedLayout>
#include<QStackedWidget>
#include"LevelOnepa.h"

class PracticePage : public QWidget
{
    Q_OBJECT
public:
    explicit PracticePage(QWidget *parent = nullptr);
    ~PracticePage();
    QPushButton *returnButton;
    QPushButton *levelOneButton;
    QPushButton *levelTwoButton;
    QPushButton *levelThreeButton;
    void updateinformation();
    QString imagePath;
    QString _password;
    QString _username;

signals:
    void returnToMainPage(); // 发送返回主页面信号

private slots:
    void on_ReturnButton_clicked();
    void on_ReturnToPracticePage();
    void showLevelOnePa();
    void readlevelone();
    void readleveltwo();
    void readlevelthree();
    void onCustomDifficultyButtonClicked();

private:
    QVBoxLayout *vLayout;
    LevelOnepa *levelOnePa;
    QStackedWidget *stackedWidget;
};
#endif // PRACTICEPAGE_H
