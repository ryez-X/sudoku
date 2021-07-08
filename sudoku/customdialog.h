#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>
#include<QTextBrowser>
#include<QBoxLayout>
namespace Ui {
class CustomDialog;
}

class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDialog(QWidget *parent = nullptr);
    ~CustomDialog();

private:
    Ui::CustomDialog *ui;
    QTextBrowser *textBrowser;
    QGridLayout*newLayout;
};

#endif // CUSTOMDIALOG_H
