#include "widget.h"
#include <QApplication>
#include<QFile>
#include"LevelOnepa.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //argc 和 argv 参数用于从命令行传递参数给应用程序
    Widget w;
    w.show();
    // LevelOnepa ah;
    // ah.show();
    // SettingsPage s;
    // s.show();
    return a.exec();
    //启动应用程序的事件循环
}
