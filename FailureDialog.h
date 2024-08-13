#ifndef FAILUREDIALOG_H
#define FAILUREDIALOG_H
#include <QDialog>

class FailureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FailureDialog(QWidget *parent = nullptr);

private:
    virtual QSize sizeHint () const;
};

#endif
