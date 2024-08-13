#ifndef PASSWORDFIND_H
#define PASSWORDFIND_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPalette>
#include <QInputDialog>

class PasswordFind : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordFind(QWidget *parent = nullptr);
    QString getNewPassword();

private:
    QLabel *titleLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;

private slots:
    void onOkClicked();
    void onCancelClicked();
};

#endif // PASSWORDFIND_H
