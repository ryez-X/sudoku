#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("数独游戏");
    setWindowIcon(QIcon("://image/sudoku.jpg"));
    resize(650,650);
    //游戏初始状态
    curState=readyGame;
    //游戏难度
    curDifficulty=easy;
    //游戏关卡
    levelMap.insert(0,1);
    levelMap.insert(1,0);
    levelMap.insert(2,0);
    curLevel=levelMap.value(curDifficulty);
    //数独
    sudoku=new Sudoku;
    customDialog=new CustomDialog;
    //菜单初始化
    MenuInit();
    //设置模型
    setupModel();
    //设置视图
    setupViews();
    //加载关卡数据
    loadLevelData();
    gridViewLoad();
    //按键初始化
    btnInit();
    //初始化定时器
    timerInit();

    //状态栏
    statusBar();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete sudoku;
    delete customDialog;
    delete numBtns;

    delete diffCombo;
    delete levelLabel;
    delete restartBtn;
    delete tipBtn;
    delete deleteBtn;
    delete pauseBtn;

    delete qLCDNumer;
    delete qtimer;
    delete qtime;

    delete gridTable;
    delete selectionModel;
}

void MainWindow::setupModel()
{
    //设置模型
    gridModel=new QStandardItemModel(GRIDNUM,GRIDNUM,this);
}

void MainWindow::setupViews()
{
    //设置视图
    gridTable=new QTableView;
    gridTable->setModel(gridModel);
    //选择模型
    selectionModel=new QItemSelectionModel(gridModel);
    gridTable->setSelectionModel(selectionModel);
    //表格头
    gridTable->horizontalHeader()->setVisible(false);
    gridTable->verticalHeader()->setVisible(false);
    gridTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    gridTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //单个选中
    gridTable->setSelectionMode(QAbstractItemView::SingleSelection);
    //样式
    gridTable->setStyleSheet("QTableView{border:3px solid #D2B48C;}");//selection-background-color:#FFFFFF;
    //自定义委托
    gridTable->setItemDelegate(new CustomDelegate(gridTable));
    //布局
    newlayout=new QGridLayout;
    newlayout->addWidget(gridTable,1,0,GRIDNUM,GRIDNUM);
    //数独大小
    for(int row=0;row<gridModel->rowCount();row++)
        gridTable->setRowHeight(row,gridTable->height()/GRIDNUM);
    for(int column=0;column<gridModel->columnCount();column++)
        gridTable->setColumnWidth(column,gridTable->width()/GRIDNUM);

    if(centralWidget()->layout())
        delete centralWidget()->layout();

    centralWidget()->setLayout(newlayout);
}

void MainWindow::MenuInit()
{
    //菜单初始化
    QMenu*option=new QMenu("选项(&O)",this);
    QAction *startGameAtn=option->addAction("开始游戏");
    startGameAtn->setShortcut(QKeySequence(tr("Ctrl+S")));

    option->addAction("新游戏",this,SLOT(NewGame()),QKeySequence(tr("Ctrl+N")));
    option->addAction("游戏提示",this,SLOT(TipGame()),QKeySequence(tr("Ctrl+T")));
    option->addAction("暂停游戏",this,SLOT(PauseGame()),QKeySequence(tr("Ctrl+P")));
    option->addAction("继续游戏",this,SLOT(PauseGame()),QKeySequence(tr("Ctrl+E")));
    option->addAction("重新开始",this,SLOT(RestartGame()),QKeySequence(tr("Ctrl+R")));
    option->addAction("提交",this,SLOT(SubmitGame()),QKeySequence(tr("Ctrl+F")));
    option->addAction("退出",this,SLOT(QuitGame()),QKeySequence(tr("Ctrl+Q")));

    QMenu*help=new QMenu("帮助(&H)",this);
    help->addAction("游戏说明",this,SLOT(HelpNote()),QKeySequence(tr("Ctrl+H")));
    help->addAction("关于",this,SLOT(AboutGame()),QKeySequence(tr("Ctrl+A")));

    //连接（两种方式）
    connect(startGameAtn,&QAction::triggered,this,&MainWindow::StartGame);

    menuBar()->addMenu(option);
    menuBar()->addMenu(help);
}

void MainWindow::loadLevelData()
{
    //加载关卡数据
    sudoku->createRandomSudokuArray(curAnswer);
    for(int row=0;row<GRIDNUM;row++)
        for(int column=0;column<GRIDNUM;column++)
            curGird[row][column]=curAnswer[row][column];
    sudoku->createRandomSudokuGame(curGird,curDifficulty);
}

void MainWindow::gridViewLoad()
{
    //视图加载
    QFont font;
    font.setBold(true);
    font.setPointSize(20);
    for(int row=0;row<gridModel->rowCount();row++)
    {
        for(int column=0;column<gridModel->columnCount();column++)
        {
            int value=curGird[row][column];
            if(value!=0)
            {
                gridModel->setData(gridModel->index(row,column,QModelIndex()),QString::number(value));
            }
            else
            {
                gridModel->setData(gridModel->index(row,column,QModelIndex()),"");
            }
            gridModel->setData(gridModel->index(row,column,QModelIndex()),font,Qt::FontRole);
            gridModel->setData(gridModel->index(row,column,QModelIndex()),QColor(getColor(value)),Qt::BackgroundRole);
            gridModel->setData(gridModel->index(row,column,QModelIndex()),Qt::AlignCenter,Qt::TextAlignmentRole);

        }
    }
    gridTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

QString MainWindow::getColor(int value)
{
    //根据值获取颜色
    switch (value) {
    case 0:
        return "#FFEBCD";
    case 1:
        return "#D1EEEE";
    case 2:
        return "#B0E2FF";
    case 3:
        return "#EEE8AA";
    case 4:
        return "#CAFF70";
    case 5:
        return "#90EE90";
    case 6:
        return "#D8BFD8";
    case 7:
        return "#00EE76";
    case 8:
        return "#7FFFD4";
    case 9:
        return "#00FFFF";
    default:
        return "FFF";
    }
}

void MainWindow::btnInit()
{
    //按键初始化
    int btnWidth=gridTable->width()/GRIDNUM;
    QFont font;
    font.setBold(false);
    font.setPointSize(15);
    //菜单按键
    pauseBtn=new QPushButton("暂停");
    pauseBtn->setFont(font);
    pauseBtn->setMaximumSize(btnWidth,btnWidth);
    pauseBtn->setStyleSheet("QPushButton{background:#94c1b1}");
    pauseBtn->setShortcut(QKeySequence(tr("P")));

    restartBtn=new QPushButton("重玩");
    restartBtn->setFont(font);
    restartBtn->setMaximumSize(btnWidth,btnWidth);
    restartBtn->setStyleSheet("QPushButton{background:#94c1b1}");
    restartBtn->setShortcut(QKeySequence(tr("R")));

    tipBtn=new QPushButton("提示");
    tipBtn->setFont(font);
    tipBtn->setMaximumSize(btnWidth,btnWidth);
    tipBtn->setStyleSheet("QPushButton{background:#94c1b1}");
    tipBtn->setShortcut(QKeySequence(tr("T")));

    deleteBtn=new QPushButton("删除");
    deleteBtn->setFont(font);
    deleteBtn->setMaximumSize(btnWidth,btnWidth);
    deleteBtn->setStyleSheet("QPushButton{background:#94c1b1}");
    deleteBtn->setShortcut(QKeySequence(tr("D")));
    //游戏难度选择框
    diffCombo=new QComboBox;
    diffCombo->setFont(font);
    diffCombo->setMaximumSize(btnWidth,btnWidth);
    QStringList diffList;
    diffList<<"简单"<<"普通"<<"困难";
    diffCombo->addItems(diffList);
    diffCombo->setStyleSheet("QComboBox{background:#94c1b1};"
                             "QComboBox-selection{background:#EEEED1}");

    //关卡标识
    levelLabel=new QLabel;
    levelLabel->setFont(font);
    levelLabel->setText("Level "+QString::number(curLevel));
    levelLabel->setStyleSheet("QLabel{border:2px solid #D2B48C;}");

    //数字按键
    numBtns=new QButtonGroup;
    for(int i=0;i<GRIDNUM;i++)
    {
        QPushButton*button=new QPushButton(QString::number(i+1));
        button->setFont(font);
        button->setMaximumSize(btnWidth,btnWidth);
        button->setStyleSheet("QPushButton{background:#A2B5CD}");
        button->setShortcut(QKeySequence(Qt::Key_1+i));
        newlayout->addWidget(button,i+1,GRIDNUM,1,1);
        numBtns->addButton(button,i);
    }
    //窗口布局
    newlayout->addWidget(restartBtn,0,0,1,1);
    newlayout->addWidget(pauseBtn,0,1,1,1);
    newlayout->addWidget(tipBtn,0,2,1,1);
    newlayout->addWidget(deleteBtn,0,3,1,1);
    newlayout->addWidget(diffCombo,0,4,1,1);
    newlayout->addWidget(levelLabel,0,5,1,2);

    //连接槽
    connect(pauseBtn,SIGNAL(clicked()),this,SLOT(PauseGame()));
    connect(restartBtn,SIGNAL(clicked()),this,SLOT(RestartGame()));
    connect(tipBtn,SIGNAL(clicked()),this,SLOT(TipGame()));
    connect(deleteBtn,SIGNAL(clicked()),this,SLOT(DeleteContent()));
    connect(numBtns,SIGNAL(buttonClicked(int)),this,SLOT(JudgeBtns(int)));

}

void MainWindow::closeEvent(QCloseEvent*event)
{
    //窗口关闭
    if(!QMessageBox::information(this,tr("提示"),tr("确定退出游戏？"),tr("确定"),tr("取消")))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::timerInit()
{
    //计时器初始化
    qtime=new QTime(0,0,0);

    qLCDNumer=new QLCDNumber;
    qLCDNumer->setDigitCount(8);
    qLCDNumer->display(qtime->toString("hh:mm:ss"));
    qLCDNumer->setStyleSheet("border:0px;color:green;");
    QFont font;
    font.setPointSize(13);
    font.setBold(true);
    qLCDNumer->setFont(font);
    newlayout->addWidget(qLCDNumer,0,8,1,2);

    //定时器
    qtimer=new QTimer;
    qtimer->setInterval(1000);
    //槽连接
    connect(this->qtimer,SIGNAL(timeout()),this,SLOT(updateDisplay()));

}

void MainWindow::updateDisplay()
{
    //计时器超时响应

    *qtime=qtime->addSecs(1);

    this->qLCDNumer->display(qtime->toString("hh:mm:ss"));

}

void MainWindow::StartGame()
{
    //开始游戏
    if(curState==GameState::readyGame||curState==GameState::finishGame)
    {
        qtime->setHMS(0,0,0);
        this->qtimer->start();
        //this->qtime=QTime::currentTime();
        curState=GameState::inGame;
    }
    else
    {
        QMessageBox::information(this,"提示","游戏已经开始");
        return;
    }

}

void MainWindow::NewGame()
{
    //新游戏
    if(!QMessageBox::information(this,"新游戏","你确定要开始一局新游戏吗？",tr("确定"),tr("取消")))
    {
        qtimer->stop();
        qtime->setHMS(0,0,0);
        this->qLCDNumer->display(qtime->toString("hh:mm:ss"));
        curDifficulty=(Difficulty)diffCombo->currentIndex();
        levelMap.insert(curDifficulty,levelMap.value(curDifficulty)+1);
        curLevel=levelMap.value(curDifficulty);
        levelLabel->setText("Level "+QString::number(curLevel));
        //加载关卡数据
        loadLevelData();
        //加载视图
        gridViewLoad();
        curState=GameState::readyGame;
    }
}

void MainWindow::TipGame()
{
    //游戏提示
    if(curState==GameState::inGame)
    {
        QModelIndex curIndex=gridTable->currentIndex();
        int row=curIndex.row();
        int column=curIndex.column();
        if(curGird[row][column]==0)
            gridModel->setData(curIndex,QString::number(curAnswer[row][column]));
    }
    else if(curState==GameState::readyGame)
    {
        QMessageBox::information(this,"提示","游戏还未开始");
        return;
    }
}

void MainWindow::DeleteContent()
{
    //删除内容
    if(curState==GameState::inGame)
    {
        QModelIndex curIndex=gridTable->currentIndex();
        int row=curIndex.row();
        int column=curIndex.column();
        if(curGird[row][column]==0)
            gridModel->setData(curIndex,QString(""));
    }
    else if(curState==GameState::pauseGame)
    {
        QMessageBox::information(this,"提示","游戏已暂停");
        return;
    }
    else if(curState==GameState::readyGame)
    {
        QMessageBox::information(this,"提示","游戏还未开始");
        return;
    }
}

void MainWindow::PauseGame()
{
    //暂停游戏
    if(curState==GameState::inGame)
    {
        pauseBtn->setText("继续");

        this->qtimer->stop();
        curState=GameState::pauseGame;
    }
    else if(curState==GameState::pauseGame)
    {
        pauseBtn->setText("暂停");
        this->qtimer->start();
        curState=GameState::inGame;
    }
    else if(curState==GameState::readyGame)
    {
        QMessageBox::information(this,"提示","游戏还未开始");
        return;
    }
}

void MainWindow::RestartGame()
{
    //重新开始本局游戏
    if(curState!=GameState::readyGame)
    {
        if(!QMessageBox::information(this,"提示","你确定要重新开始本局游戏？\n当前记录将丢失。",tr("确定"),tr("取消")))
        {
            this->qtimer->stop();
            this->qtime->setHMS(0,0,0);
            this->qLCDNumer->display(qtime->toString("hh:mm:ss"));
            //初始化数独视图
            gridViewLoad();
            this->qtimer->start();
            curState=GameState::inGame;
        }
    }
    else
    {
        QMessageBox::information(this,"提示","请开始游戏");
        return;
    }
}

void MainWindow::SubmitGame()
{
    //提交游戏
    if(curState==GameState::inGame||curState==GameState::pauseGame)
    {
        for(int row=0;row<GRIDNUM;row++)
            for(int column=0;column<GRIDNUM;column++)
                if(gridModel->item(row,column)->text()!=QString::number(curAnswer[row][column]))
                {
                    QMessageBox::information(this,"提示","很遗憾！答案错误！");
                    return;
                }
        this->qtimer->stop();
        QMessageBox::information(this,"提示","恭喜你！答案正确！");
        curState=GameState::finishGame;
    }
    else if(curState==GameState::finishGame)
    {
        QMessageBox::information(this,"提示","游戏已经提交");
        return;
    }
    else if(curState==GameState::readyGame)
    {
        QMessageBox::information(this,"提示","游戏还未开始");
        return;
    }
}

void MainWindow::QuitGame()
{
    //退出游戏
    if(!QMessageBox::information(this,tr("提示"),tr("确定退出游戏？"),tr("确定"),tr("取消")))
    {
        QApplication*app;
        app->exit(0);
    }
}


void MainWindow::HelpNote()
{
    //游戏说明
    customDialog->show();
}

void MainWindow::AboutGame()
{
    //关于游戏
    QMessageBox::information(this,"关于数独","by ryez.");
}

void MainWindow::JudgeBtns(int btnID)
{
    //数字按键响应
    if(curState==GameState::inGame)
    {
        QModelIndex curIndex=gridTable->currentIndex();
        int row=curIndex.row();
        int column=curIndex.column();
        if(curGird[row][column]==0)
            gridModel->setData(curIndex,QString::number(btnID+1));
    }
    else if(curState==GameState::readyGame)
    {
        QMessageBox::information(this,"提示","游戏还未开始");
        return;
    }
}
