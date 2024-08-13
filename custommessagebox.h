#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsOpacityEffect>

class CustomMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit CustomMessageBox(const QString &message, QWidget *parent = nullptr);

private:
    QLabel *messageLabel;
    QPushButton *okButton;

private slots:
    void onOkClicked();
};

#endif // CUSTOMMESSAGEBOX_H
