#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QGraphicsOpacityEffect>
#include <QPalette>

class InputDialog : public QInputDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);

    QString getTextValue();

private:
    void setupUI();
};

#endif // INPUTDIALOG_H
