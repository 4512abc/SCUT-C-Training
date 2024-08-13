#ifndef MSGBOX_H
#define MSGBOX_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MsgBox : public QDialog
{
    Q_OBJECT

public:
    explicit MsgBox(QWidget *parent = nullptr);
    ~MsgBox();

private slots:
    void onYesClicked();
    void onNoClicked();

private:
    QLabel *messageLabel;
    QPushButton *yesButton;
    QPushButton *noButton;

    void setupUI();
};

#endif // MSGBOX_H
