#include "customdialog.h"
#include "ui_customdialog.h"

CustomDialog::CustomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomDialog)
{
    ui->setupUi(this);
    resize(600,300);
    setWindowTitle("游戏说明");
    QFont font;
    font.setBold(true);
    font.setPointSize(15);
    textBrowser=new QTextBrowser;
    QString gameNote="1.开始游戏请选择 选项(Alt+O)->开始游戏(Ctrl+S)\n"
                     "2.更换游戏难度或下一关请选择 选项(Alt+O)->新游戏(Ctrl+N)\n"
                     "3.填入数字请选择按键或快捷键（例如：1对应快捷键1..）\n"
                     "4.删除数字请选择按键或快捷键 D\n";
    textBrowser->setText(gameNote);
    textBrowser->setFont(font);

    newLayout=new QGridLayout;
    newLayout->addWidget(textBrowser,0,0);
    this->setLayout(newLayout);
}

CustomDialog::~CustomDialog()
{
    delete ui;
    delete textBrowser;
}
