#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMessageBox>
#include <QCloseEvent>
#include <math.h>
#include <QBoxLayout>

#include"sudoku.h"
#include"customdialog.h"
#include"customdelegate.h"

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QAbstractItemView;
class QItemSelectionModel;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //菜单初始化
    void MenuInit();
    //加载关卡数据
    void loadLevelData();
    //视图加载
    void gridViewLoad();
    //根据值获取颜色
    QString getColor(int);
    //按键初始化
    void btnInit();
    //窗口关闭
    void closeEvent(QCloseEvent*);
public:
    //计时器初始化
    void timerInit();
private slots:
    //计时器超时响应
    void updateDisplay();
private:
    //设置模型
    void setupModel();
    //设置视图
    void setupViews();
private slots:
    //开始游戏
    void StartGame();
    //新游戏
    void NewGame();
    //游戏提示
    void TipGame();
    //删除内容
    void DeleteContent();
    //暂停游戏
    void PauseGame();
    //重新开始本局游戏
    void RestartGame();
    //提交游戏
    void SubmitGame();
    //退出游戏
    void QuitGame();
    //游戏说明
    void HelpNote();
    //关于游戏
    void AboutGame();
    //数字按键响应
    void JudgeBtns(int);
private:
    //数据模型
    QStandardItemModel*gridModel;
    //表格视图
    QTableView *gridTable;
    //选择模型
    QItemSelectionModel*selectionModel;
private:
    //计时器部分
    QLCDNumber *qLCDNumer;
    QTimer *qtimer;
    QTime *qtime;
    //图标菜单按键
    QPushButton*pauseBtn;
    QPushButton*restartBtn;
    QPushButton*tipBtn;
    QPushButton*deleteBtn;
    //游戏难度选择
    QComboBox*diffCombo;
    //关卡标识
    QLabel*levelLabel;
    //选填数字按键
    QButtonGroup*numBtns;
private:
    Ui::MainWindow *ui;
    //布局
    QGridLayout *newlayout;
    CustomDialog*customDialog;
private:
    //数独数据
    Sudoku *sudoku;
    //游戏状态
    GameState curState;
    //游戏难度
    Difficulty curDifficulty;
    //游戏关卡
    QMap<int,int>levelMap;
    int curLevel;
    //当前关卡答案及游戏
    int curAnswer[GRIDNUM][GRIDNUM],curGird[GRIDNUM][GRIDNUM];
};

#endif // MAINWINDOW_H
