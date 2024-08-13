#include "CustomInputDialog.h"
#include "qpushbutton.h"
#include <QDoubleValidator>
#include <QGraphicsOpacityEffect>
#include <QPalette>

CustomInputDialog::CustomInputDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("自定义输入");
    setFixedSize(600, 300);  // 调整对话框大小
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 无边框对话框

    // 设置背景颜色和透明度
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0, 200)); // 黑色背景，透明度200
    setAutoFillBackground(true);
    setPalette(palette);

    // 设置透明效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8); // 设置透明度为80%
    setGraphicsEffect(opacityEffect);

    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 26px; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);

    valueLabel = new QLabel(tr("数值:"), this); // 标签内容改为数值
    valueLabel->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 26px; color: white;");
    valueLabel->setAlignment(Qt::AlignCenter);

    valueLineEdit = new QLineEdit(this);
    valueLineEdit->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 26px;");
    valueLineEdit->setAlignment(Qt::AlignCenter);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    // 设置按钮样式
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setStyleSheet("QPushButton {"
                            "background-color: #FF0000; border: none; border-radius: 10px; "
                            "font-family: 'Microsoft YaHei'; font-size: 24px; color: white;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #CC0000;"
                            "}");

    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    cancelButton->setStyleSheet("QPushButton {"
                                "background-color: #808080; border: none; border-radius: 10px; "
                                "font-family: 'Microsoft YaHei'; font-size: 24px; color: white;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #696969;"
                                "}");

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(valueLabel);
    mainLayout->addWidget(valueLineEdit);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

CustomInputDialog::~CustomInputDialog()
{
}

double CustomInputDialog::getDoubleValue(const QString &title, const QString &label, double defaultValue, double minValue, double maxValue)
{
    setWindowTitle(title);
    titleLabel->setText(label);

    // 初始化输入框的文本为默认值
    QString text = QString::number(defaultValue, 'f', 1);
    valueLineEdit->setText(text);

    // 设置输入框验证器，限定输入范围和小数位数
    QDoubleValidator *validator = new QDoubleValidator(minValue, maxValue, 1, this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    valueLineEdit->setValidator(validator);

    while (true)
    {
        if (exec() == QDialog::Accepted) {
            bool ok;
            double value = valueLineEdit->text().toDouble(&ok);
            if (ok && value >= minValue && value <= maxValue) {
                return value;
            }
        } else
        {
            return -1; // 如果取消，则返回-1
        }
    }
}
