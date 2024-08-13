#ifndef CUSTOMINPUTDIALOG_H
#define CUSTOMINPUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>

class CustomInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomInputDialog(QWidget *parent = nullptr);
    ~CustomInputDialog();

    double getDoubleValue(const QString &title, const QString &label, double defaultValue, double minValue, double maxValue);

private:
    QLabel *titleLabel;
    QLabel *valueLabel;
    QLineEdit *valueLineEdit;
    QDialogButtonBox *buttonBox;
    //一个标准类，提供了一组按钮以供使用
    QVBoxLayout *mainLayout;
};

#endif // CUSTOMINPUTDIALOG_H
