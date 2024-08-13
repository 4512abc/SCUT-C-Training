#ifndef USERNAMEFIND_H
#define USERNAMEFIND_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPalette>

class UsernameFind : public QDialog
{
    Q_OBJECT

public:
    explicit UsernameFind(QWidget *parent = nullptr);
    QString getUsername();

private:
    QLabel *titleLabel;
    QLineEdit *usernameLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;

private slots:
    void onOkClicked();
    void onCancelClicked();
};

#endif // USERNAMEFIND_H
