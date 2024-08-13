#ifndef EXITDIALOG_H
#define EXITDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ExitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExitDialog(QWidget *parent = nullptr);

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    QLabel *messageLabel;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
};

#endif // EXITDIALOG_H
