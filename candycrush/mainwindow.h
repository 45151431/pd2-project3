#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QLabel>
#include <cstring>
#include <cstdlib>
#include <ctime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void game_init();//遊戲開始動畫
    void t_eli_start();//限時模式的開始
    void s_eli_start();//限步模式的開始
    void output();//在視窗上顯現
    void change();//交換
    void anim_init();//動畫指標的delete
    void g_anim_create();//g_label的動畫
    void animCreate(int num, int Dura, int x1, int y1, int x2, int y2, int size1, int size2, int group);//button的動畫
    void Remove_init();
    void DFS(int i, int j);//利用DFS找相連
    void dropnew();//掉新糖果

    bool IsRemove();//判斷盤面還能否消除
    bool IsSpecialMove(int color1, int color2);//判斷移動的糖果中有沒有特殊糖果
    bool IsSpecialCreate();//判斷是否有待生成的特殊糖果
    bool IsSpecialRemove();//判斷是否有待消除的特殊糖果
    bool IsStar(int color);//判斷是否產生星星
    bool IsBomb(int color);//判斷是否產生炸彈
    bool IsVelimi(int color);//判斷是否產生垂直消除
    bool IsHelimi(int color);//判斷是否產生水平消除

    ~MainWindow();

signals:
    void quit(int star, int score);

public slots:
    virtual void start();//遊戲開始/重新開始
    void g_anim_hide();//隱藏g_label
    void select(int);//選擇交換的對象
    void rechange();//換回來
    void SpecialMove();//特殊糖果消除
    void Remove();//消除
    void SpecialRemove();
    void OutOfEvent();
    void t_eli_game_update();
    void s_eli_game_update();
    void star_count();
    void EndGame();

private:
    Ui::MainWindow *ui;
    QLabel *c_label, *s_label, *m_label, *e_label, *t_label, *g_label, *star_label[3], *back_label[10][10];
    QPushButton *button[10][10];
    QIcon Icon[18];
    QSignalMapper *signalMapper;
    QPropertyAnimation *anim[100], *g_anim;
    QParallelAnimationGroup *p_group[9];
    QSequentialAnimationGroup *s_group;
    QTimer *timer;
    int score, star;
    int time_cnt, step_cnt, drop_cnt;
    int s1, s2;
    int map[10][10];
    int Special[10][10];
    bool link[10][10];
    bool rem[10][10];
    bool BombRemove[10][10];
    bool VelimiRemove[10][10];
    bool HelimiRemove[10][10];
    bool connect_check;
    bool IsGameOver;
    bool IsSelectone;
    bool IsInEvent;
    bool check;
};

class colorgroup{
public:
    colorgroup(int color)
    {
        if(color==0)
            i=0;
        else if(color==1||color==5||color==9||color==13)
            i=1;
        else if(color==2||color==6||color==10||color==14)
            i=2;
        else if(color==3||color==7||color==11||color==15)
            i=3;
        else if(color==4||color==8||color==12||color==16)
            i=4;
        else if(color==17)
            i=5;
        else
            i=6;
    }
    bool operator ==(const colorgroup & t)
    {
        if(i==t.i)
            return true;
        return false;
    }
    int i;
};

#endif // MAINWINDOW_H
