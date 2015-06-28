#include "mainwindow.h"
#include "ui_mainwindow.h"

#define change_speed 200
#define remove_speed 200
#define create_speed 50
#define drop_speed 300
#define game_time 80
#define game_step 30
#define star_value 270

void objectDestroyed(QObject* obj = 0);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(NULL));

/***設定一些背景Label和Button***/
    c_label=new QLabel(ui->centralWidget);
    c_label->setPixmap(QPixmap(":/background/background/candycrush.png"));
    c_label->setScaledContents(true);

    s_label=new QLabel(ui->centralWidget);
    s_label->setPixmap(QPixmap(":/background/background/score.png"));
    s_label->setScaledContents(true);

    m_label=new QLabel(ui->centralWidget);
    m_label->setGeometry(QRect(390, 200, 100, 60));
    m_label->setStyleSheet("qproperty-alignment: 'AlignCenter | AlignCenter';");
    m_label->setFont(QFont("Courier New", 14, QFont::Bold));

    e_label=ui->eli;
    t_label=ui->type;

    for(int i=0; i<10; i++)//做出半透明的背景Label
        for(int j=0; j<10; j++)
        {
            back_label[i][j]=new QLabel(ui->centralWidget);
            back_label[i][j]->setGeometry(j*30+10, i*30+80, 29, 29);
            back_label[i][j]->setStyleSheet("background-color:rgba(200,200,255,200)");
        }
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
        {
            button[i][j]=new QPushButton(ui->centralWidget);
            button[i][j]->setStyleSheet("background-color:transparent");
        }
    g_label=new QLabel(ui->centralWidget);
    g_label->setScaledContents(true);

    for(int i=0; i<3; i++)
    {
        star_label[i]=new QLabel(ui->centralWidget);
        star_label[i]->setScaledContents(true);
        star_label[i]->setPixmap(QPixmap(":/background/background/star.png"));
    }
/***加入Icon***/
    Q_INIT_RESOURCE(res);
    Icon[1].addFile(":/Icon/Icon/1_curve_candy.png");
    Icon[2].addFile(":/Icon/Icon/2_circle_candy.png");
    Icon[3].addFile(":/Icon/Icon/3_hexagon_candy.png");
    Icon[4].addFile(":/Icon/Icon/4_rect_candy.png");
    Icon[5].addFile(":/Icon/Icon/5_curve_candy_v.png");
    Icon[6].addFile(":/Icon/Icon/6_circle_candy_v.png");
    Icon[7].addFile(":/Icon/Icon/7_hexagon_candy_v.png");
    Icon[8].addFile(":/Icon/Icon/8_rect_candy_v.png");
    Icon[9].addFile(":/Icon/Icon/9_curve_candy_h.png");
    Icon[10].addFile(":/Icon/Icon/10_circle_candy_h.png");
    Icon[11].addFile(":/Icon/Icon/11_hexagon_candy_h.png");
    Icon[12].addFile(":/Icon/Icon/12_rect_candy_h.png");
    Icon[13].addFile(":/Icon/Icon/13_curve_candy_b.png");
    Icon[14].addFile(":/Icon/Icon/14_circle_candy_b.png");
    Icon[15].addFile(":/Icon/Icon/15_hexagon_candy_b.png");
    Icon[16].addFile(":/Icon/Icon/16_rect_candy_b.png");
    Icon[17].addFile(":/Icon/Icon/17_star.png");
/***連結Button和SignalMapper***/
    signalMapper=new QSignalMapper(this);
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
        {
            signalMapper->setMapping(button[i][j], i*10+j);
            connect(button[i][j], SIGNAL(clicked()), signalMapper, SLOT(map()));
        }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(select(int)));
/***連結reset和menu按鈕***/
    connect(ui->menu, SIGNAL(clicked()), this, SLOT(EndGame()));
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(start()));
/***連結動畫和Function***/
    for(int i=0; i<9; i++)
        p_group[i]=new QParallelAnimationGroup;
    connect(p_group[0], SIGNAL(finished()), this, SLOT(SpecialMove()));
    connect(p_group[1], SIGNAL(finished()), this, SLOT(Remove()));
    connect(p_group[2], SIGNAL(finished()), this, SLOT(rechange()));
    connect(p_group[3], SIGNAL(finished()), this, SLOT(OutOfEvent()));
    connect(p_group[4], SIGNAL(finished()), this, SLOT(SpecialRemove()));
    connect(p_group[5], SIGNAL(finished()), this, SLOT(SpecialRemove()));
    connect(p_group[6], SIGNAL(finished()), this, SLOT(SpecialRemove()));
    connect(p_group[7], SIGNAL(finished()), this, SLOT(OutOfEvent()));
    connect(p_group[8], SIGNAL(finished()), this, SLOT(start()));
    s_group=new QSequentialAnimationGroup;
    connect(s_group, SIGNAL(finished()), this, SLOT(EndGame()));
/***將PropertyAnimation指向NULL***/
    for(int i=0; i<100; i++)
        anim[i]=NULL;
    g_anim=NULL;
/***初始化***/
    timer=new QTimer(this);
    connect_check=true;
    star=0;
    score=0;
}
void MainWindow::game_init()//遊戲開始動畫
{
    anim_init();
    p_group[8]->clear();
    anim[0]=new QPropertyAnimation(c_label, "geometry");
    anim[0]->setDuration(1000);
    anim[0]->setStartValue(QRect(-220, 0, 220, 60));
    anim[0]->setEndValue(QRect(0, 0, 220, 60));
    p_group[8]->addAnimation(anim[0]);

    anim[1]=new QPropertyAnimation(s_label, "geometry");
    anim[1]->setDuration(1000);
    anim[1]->setStartValue(QRect(500, 200, 220, 60));
    anim[1]->setEndValue(QRect(280, 200, 220, 60));
    p_group[8]->addAnimation(anim[1]);

    g_label->setPixmap(QPixmap(":/background/background/gamestart.png"));
    g_anim_create();
    connect(g_anim, SIGNAL(finished()), this, SLOT(g_anim_hide()));
    g_anim->start();

    p_group[8]->start();

    IsGameOver=true;
    IsInEvent=false;
    e_label->hide();
    t_label->hide();
    m_label->hide();
    ui->menu->hide();
    ui->reset->hide();
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
            button[i][j]->hide();
    for(int i=0; i<3; i++)
        star_label[i]->hide();
    do
    {
        for(int i=0; i<10; i++)
            for(int j=0; j<10; j++)
                map[i][j]=rand()%4+1;
    }while(IsRemove());
    return;
}
void MainWindow::t_eli_start()//限時模式的開始
{
    if(IsInEvent)
        return;
    anim_init();
    if(!IsGameOver)
        do
        {
            for(int i=0; i<10; i++)
                for(int j=0; j<10; j++)
                    map[i][j]=rand()%4+1;
        }while(IsRemove());
    IsGameOver=false;
    IsSelectone=false;
    if(connect_check)
    {
        connect(timer, SIGNAL(timeout()), this, SLOT(t_eli_game_update()));
        connect_check=false;
    }
    output();
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
            button[i][j]->show();
    time_cnt=game_time;
    e_label->setText(QVariant(game_time).toString());
    score=0;
    star=0;
    m_label->setText(QVariant(score).toString());
    t_label->setText("time");
    e_label->show();
    m_label->show();
    t_label->show();
    ui->menu->show();
    ui->reset->show();
    timer->start(1000);
    return;
}
void MainWindow::s_eli_start()//限步模式的開始
{
    if(IsInEvent)
        return;
    anim_init();
    if(!IsGameOver)
        do
        {
            for(int i=0; i<10; i++)
                for(int j=0; j<10; j++)
                    map[i][j]=rand()%4+1;
        }while(IsRemove());
    IsGameOver=false;
    IsSelectone=false;
    if(connect_check)
    {
        connect(p_group[0], SIGNAL(finished()), this, SLOT(s_eli_game_update()));
        connect(p_group[1], SIGNAL(finished()), this, SLOT(s_eli_game_update()));
        connect_check=false;
    }
    output();
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
            button[i][j]->show();
    step_cnt=game_step;
    e_label->setText((QVariant(game_step).toString()));
    score=0;
    m_label->setText(QVariant(score).toString());
    t_label->setText("step");
    e_label->show();
    m_label->show();
    t_label->show();
    ui->menu->show();
    ui->reset->show();
    return;
}
void MainWindow::start()//遊戲開始/重新開始
{
    return;
}
void MainWindow::output()//在視窗上顯現
{
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
        {
            button[i][j]->setGeometry(j*30+10, i*30+80, 30, 30);
            if(map[i][j]==0)
                button[i][j]->setGeometry(j*30+10, i*30+80, 0, 0);
            else
            {
                button[i][j]->setIcon(Icon[map[i][j]]);
                button[i][j]->setIconSize(QSize(30, 30));
            }
        }
    return;
}
void MainWindow::anim_init()//動畫指標的delete
{
    for(int i=0; i<100; i++)
        if(anim[i]!=NULL)
        {
            delete anim[i];
            anim[i]=NULL;
        }
    return;
}
void MainWindow::g_anim_create()//g_label的動畫
{
    delete g_anim;
    g_label->show();
    g_anim=new QPropertyAnimation(g_label, "geometry");
    g_anim->setDuration(1000);
    g_anim->setStartValue(QRect(60, 150, 200, 60));
    g_anim->setEndValue(QRect(10, 190, 300, 80));
    return;
}
void MainWindow::g_anim_hide()//隱藏g_label
{
    g_label->hide();
    return;
}
void MainWindow::animCreate(int num, int Dura, int x1, int y1, int x2, int y2, int size1, int size2, int group)//button的動畫
{
    anim[num]=new QPropertyAnimation(button[num/10][num%10], "geometry");
    anim[num]->setDuration(Dura);
    anim[num]->setStartValue(QRect(x1, y1, size1, size1));
    anim[num]->setEndValue(QRect(x2, y2, size2, size2));
    p_group[group]->addAnimation(anim[num]);
    return;
}
void MainWindow::select(int selectbutton)//選擇交換的對象
{
    if(!IsGameOver&&!IsInEvent)
    {
        if(!IsSelectone)
        {
            s1=selectbutton;
            IsSelectone=true;
            return;
        }
        IsInEvent=true;
        s2=selectbutton;
        int tmp=s2-s1;
        if(tmp!=1&&tmp!=-1&&tmp!=10&&tmp!=-10)
        {
            s1=selectbutton;
            IsInEvent=false;
            return;
        }
        IsSelectone=false;
        change();
    }
    return;
}
void MainWindow::change()//交換
{
    drop_cnt=0;
    int tmp=map[s1/10][s1%10];
    map[s1/10][s1%10]=map[s2/10][s2%10];
    map[s2/10][s2%10]=tmp;
    if(IsSpecialMove(colorgroup(map[s1/10][s1%10]).i, colorgroup(map[s2/10][s2%10]).i))
    {
        animCreate(s1, change_speed, (s1%10)*30+10, (s1/10)*30+80, (s2%10)*30+10, (s2/10)*30+80, 30, 30, 0);
        animCreate(s2, change_speed, (s2%10)*30+10, (s2/10)*30+80, (s1%10)*30+10, (s1/10)*30+80, 30, 30, 0);
        p_group[0]->start();
    }
    else if(IsRemove())
    {
        animCreate(s1, change_speed, (s1%10)*30+10, (s1/10)*30+80, (s2%10)*30+10, (s2/10)*30+80, 30, 30, 1);
        animCreate(s2, change_speed, (s2%10)*30+10, (s2/10)*30+80, (s1%10)*30+10, (s1/10)*30+80, 30, 30, 1);
        p_group[1]->start();
    }
    else
    {
        animCreate(s1, change_speed, (s1%10)*30+10, (s1/10)*30+80, (s2%10)*30+10, (s2/10)*30+80, 30, 30, 2);
        animCreate(s2, change_speed, (s2%10)*30+10, (s2/10)*30+80, (s1%10)*30+10, (s1/10)*30+80, 30, 30, 2);
        p_group[2]->start();
    }
    return;
}
void MainWindow::rechange()//換回來
{
    anim_init();
    p_group[2]->clear();
    int tmp=map[s1/10][s1%10];
    map[s1/10][s1%10]=map[s2/10][s2%10];
    map[s2/10][s2%10]=tmp;
    animCreate(s1, change_speed, (s2%10)*30+10, (s2/10)*30+80, (s1%10)*30+10, (s1/10)*30+80, 30, 30, 3);
    animCreate(s2, change_speed, (s1%10)*30+10, (s1/10)*30+80, (s2%10)*30+10, (s2/10)*30+80, 30, 30, 3);
    p_group[3]->start();
    return;
}
void MainWindow::Remove_init()
{
    memset(rem, false, sizeof(rem));
    memset(BombRemove, false, sizeof(BombRemove));
    memset(VelimiRemove, false, sizeof(VelimiRemove));
    memset(HelimiRemove, false, sizeof(HelimiRemove));
    memset(Special, 0, sizeof(Special));
    output();
    return;
}
void MainWindow::SpecialMove()//特殊糖果消除
{
    anim_init();
    p_group[0]->clear();
    Remove_init();
    int color1=colorgroup(map[s1/10][s1%10]).i;
    int color2=colorgroup(map[s2/10][s2%10]).i;
    if(color1==5 && color2==5)
        for(int i=0; i<10; i++)
            for(int j=0; j<10; j++)
            {
                score++;
                map[i][j]=0;
                animCreate(i*10+j, remove_speed, j*30+10, i*30+80, j*30+25, i*30+95, 30, 0, 4);
            }
    if(color1==5 && color2!=5)
    {
        score++;
        map[s1/10][s1%10]=0;
        animCreate(s1, remove_speed, (s1%10)*30+10, (s1/10)*30+80, (s1%10)*30+25, (s1/10)*30+95, 30, 0, 4);
        for(int i=0; i<10; i++)
            for(int j=0; j<10; j++)
                if(colorgroup(map[i][j])==color2)
                {
                    if(map[i][j]<17 && map[i][j]>12)
                        BombRemove[i][j]=true;
                    else if(map[i][j]<13 && map[i][j]>8)
                        HelimiRemove[i][j]=true;
                    else if(map[i][j]<9 && map[i][j]>4)
                        VelimiRemove[i][j]=true;
                    score++;
                    map[i][j]=0;
                    animCreate(i*10+j, remove_speed, j*30+10, i*30+80, j*30+25, i*30+95, 30, 0, 4);
                }
    }
    if(color2==5 && color1!=5)
    {
        map[s2/10][s2%10]=0;
        animCreate(s2, remove_speed, (s2%10)*30+10, (s2/10)*30+80, (s2%10)*30+25, (s2/10)*30+95, 30, 0, 4);
        for(int i=0; i<10; i++)
            for(int j=0; j<10; j++)
                if(colorgroup(map[i][j])==color1)
                {
                    if(map[i][j]<17 && map[i][j]>12)
                        BombRemove[i][j]=true;
                    else if(map[i][j]<13 && map[i][j]>8)
                        HelimiRemove[i][j]=true;
                    else if(map[i][j]<9 && map[i][j]>4)
                        VelimiRemove[i][j]=true;
                    score++;
                    map[i][j]=0;
                    animCreate(i*10+j, remove_speed, j*30+10, i*30+80, j*30+25, i*30+95, 30, 0, 4);
                }
    }
    p_group[4]->start();
    return;
}
void MainWindow::Remove()//消除
{
    anim_init();
    p_group[1]->clear();
    Remove_init();
    for(int i=9; i>=0; i--)
        for(int j=0; j<10; j++)
            if(rem[i][j]==false)
            {
                memset(link, false, sizeof(link));
                check=false;
                DFS(i, j);
                if(check==true)
                {
                    if(IsStar(map[i][j]));
                    else if(IsBomb(colorgroup(map[i][j]).i));
                    else if(IsVelimi(colorgroup(map[i][j]).i));
                    else if(IsHelimi(colorgroup(map[i][j]).i));
                    for(int m=0; m<10; m++)
                        for(int n=0; n<10; n++)
                            if(link[m][n])
                                rem[m][n]=true;
                }
            }
    for(int i=9; i>=0; i--)
        for(int j=0; j<10; j++)
            if(rem[i][j]==true)
            {
                if(map[i][j]<17 && map[i][j]>12)
                    BombRemove[i][j]=true;
                else if(map[i][j]<13 && map[i][j]>8)
                    HelimiRemove[i][j]=true;
                else if(map[i][j]<9 && map[i][j]>4)
                    VelimiRemove[i][j]=true;
                score++;
                map[i][j]=0;
                animCreate(i*10+j, remove_speed, j*30+10, i*30+80, j*30+25, i*30+95, 30, 0, 5);
            }
    p_group[5]->start();
    return;
}
void MainWindow::DFS(int i, int j)//利用DFS找相連
{
    link[i][j]=true;
    if(!link[i-1][j] && i-1>=0 && colorgroup(map[i][j])==colorgroup(map[i-1][j]))
        if((i-2>=0 && colorgroup(map[i][j])==colorgroup(map[i-2][j])) || (i+1<10 && colorgroup(map[i][j])==colorgroup(map[i+1][j])))
        {
            check=true;
            DFS(i-1, j);
        }
    if(!link[i][j-1] && j-1>=0 && colorgroup(map[i][j])==colorgroup(map[i][j-1]))
        if((j-2>=0 && colorgroup(map[i][j])==colorgroup(map[i][j-2])) || (j+1<10 && colorgroup(map[i][j])==colorgroup(map[i][j+1])))
        {
            check=true;
            DFS(i, j-1);
        }
    if(!link[i+1][j] && i+1<10 && colorgroup(map[i][j])==colorgroup(map[i+1][j]))
        if((i-1>=0 && colorgroup(map[i][j])==colorgroup(map[i-1][j])) || (i+2<10 && colorgroup(map[i][j])==colorgroup(map[i+2][j])))
        {
            check=true;
            DFS(i+1, j);
        }
    if(!link[i][j+1] && j+1<10 && colorgroup(map[i][j])==colorgroup(map[i][j+1]))
        if((j-1>=0 && colorgroup(map[i][j])==colorgroup(map[i][j-1])) || (j+2<10 && colorgroup(map[i][j])==colorgroup(map[i][j+2])))
        {
            check=true;
            DFS(i, j+1);
        }
    return;
}
void MainWindow::SpecialRemove()
{
    anim_init();
    p_group[4]->clear();
    p_group[5]->clear();
    p_group[6]->clear();
    if(IsSpecialCreate() || IsSpecialRemove())
    {
        for(int i=0; i<10; i++)
            for(int j=0; j<10; j++)
            {
                if(BombRemove[i][j])
                {
                    BombRemove[i][j]=false;
                    for(int m=i-1; m<=i+1; m++)
                        for(int n=j-1; n<=j+1; n++)
                            if(map[m][n])
                            {
                                if(m<0 || m>9 || n<0 || n>9)
                                    continue;
                                if(map[m][n]<17&&map[m][n]>12)
                                    BombRemove[m][n]=true;
                                else if(map[m][n]<13&&map[m][n]>8)
                                    HelimiRemove[m][n]=true;
                                else if(map[m][n]<9&&map[m][n]>4)
                                    VelimiRemove[m][n]=true;
                                score++;
                                map[m][n]=0;
                                animCreate(m*10+n, remove_speed, n*30+10, m*30+80, n*30+25, m*30+95, 30, 0, 6);
                            }
                }
                else if(VelimiRemove[i][j])
                {
                    VelimiRemove[i][j]=false;
                    for(int m=0; m<10; m++)
                        if(map[m][j])
                        {
                            if(map[m][j]<17&&map[m][j]>12)
                                BombRemove[m][j]=true;
                            else if(map[m][j]<13&&map[m][j]>8)
                                HelimiRemove[m][j]=true;
                            else if(map[m][j]<9&&map[m][j]>4)
                                VelimiRemove[m][j]=true;
                            score++;
                            map[m][j]=0;
                            animCreate(m*10+j, remove_speed, j*30+10, m*30+80, j*30+25, m*30+95, 30, 0, 6);
                        }
                }
                else if(HelimiRemove[i][j])
                {
                    HelimiRemove[i][j]=false;
                    for(int n=0; n<10; n++)
                        if(map[i][n])
                        {
                            if(map[i][n]<17&&map[i][n]>12)
                                BombRemove[i][n]=true;
                            else if(map[i][n]<13&&map[i][n]>8)
                                HelimiRemove[i][n]=true;
                            else if(map[i][n]<9&&map[i][n]>4)
                                VelimiRemove[i][n]=true;
                            score++;
                            map[i][n]=0;
                            animCreate(i*10+n, remove_speed, n*30+10, i*30+80, n*30+25, i*30+95, 30, 0, 6);
                        }
                }
            }
        for(int i=0; i<10; i++)
            for(int j=0; j<10; j++)
                if(Special[i][j]!=0)
                {
                    map[i][j]=Special[i][j];
                    button[i][j]->setIcon(Icon[map[i][j]]);
                    animCreate(i*10+j, create_speed, j*30+25, i*30+95, j*30+10, i*30+80, 0, 30, 6);
                    Special[i][j]=0;
                }
        p_group[6]->start();
    }
    else
        dropnew();
    return;
}
void MainWindow::dropnew()//掉新糖果
{
    output();
    drop_cnt++;
    if(drop_cnt==5)
    {
        g_label->setPixmap(QPixmap(":/background/background/sosweet.png"));
        g_anim_create();
        connect(g_anim, SIGNAL(finished()), this, SLOT(g_anim_hide()));
        g_anim->start();
    }
    for(int j=0; j<10; j++)
        for(int i=9; i>=0; i--)
            if(map[i][j]==0)
                for(int k=i-1; k>=0; k--)
                    if(map[k][j]!=0)
                    {
                        map[i][j]=map[k][j];
                        map[k][j]=0;
                        button[i][j]->setIcon(Icon[map[i][j]]);
                        animCreate(i*10+j, drop_speed, j*30+10, k*30+80, j*30+10, i*30+80, 30, 30, 7);
                        break;
                    }
    for(int j=0; j<10; j++)
        for(int i=0; i<10; i++)
            if(map[i][j]==0)
            {
                map[i][j]=rand()%4+1;
                button[i][j]->setIcon(Icon[map[i][j]]);
                animCreate(i*10+j, drop_speed, j*30+10, 50, j*30+10, i*30+80, 30, 30, 7);
            }
    p_group[7]->start();
    return;
}
void MainWindow::OutOfEvent()
{
    anim_init();
    p_group[3]->clear();
    p_group[7]->clear();
    output();
    if(IsRemove())
        Remove();
    else
    {
        m_label->setText(QVariant(score).toString());
        IsInEvent=false;
        if(IsGameOver)
        {
            g_label->setPixmap(QPixmap(":/background/background/finished.png"));
            g_anim_create();
            connect(g_anim, SIGNAL(finished()), this, SLOT(star_count()));
            g_anim->start();
        }
    }
    return;
}
void MainWindow::t_eli_game_update()
{
    time_cnt--;
    if(time_cnt==0)
    {
        timer->stop();
        e_label->setText(QVariant(time_cnt).toString());
        IsGameOver=true;
        ui->menu->hide();
        ui->reset->hide();
        if(!IsInEvent)
        {
            g_label->setPixmap(QPixmap(":/background/background/finished.png"));
            g_anim_create();
            connect(g_anim, SIGNAL(finished()), this, SLOT(star_count()));
            g_anim->start();
        }
    }
    else
    {
        e_label->setText(QVariant(time_cnt).toString());
        timer->start(1000);
    }
    return;
}
void MainWindow::s_eli_game_update()
{
    step_cnt--;
    if(step_cnt==0)
    {
        e_label->setText(QVariant(step_cnt).toString());
        IsGameOver=true;
        ui->menu->hide();
        ui->reset->hide();
        if(!IsInEvent)
        {
            g_label->setPixmap(QPixmap(":/background/background/finished.png"));
            g_anim_create();
            connect(g_anim, SIGNAL(finished()), this, SLOT(star_count()));
            g_anim->start();
        }
    }
    else
        e_label->setText(QVariant(step_cnt).toString());
    return;
}
void MainWindow::star_count()
{
    anim_init();
    s_group->clear();
    if(score<star_value)
        star=0;
    else
    {
        if(score>=star_value)
        {
            star=1;
            star_label[0]->setGeometry(QRect(0,0,0,0));
            star_label[0]->show();
            anim[0]=new QPropertyAnimation(star_label[0], "geometry");
            anim[0]->setDuration(1000);
            anim[0]->setStartValue(QRect(285, 135, 100, 100));
            anim[0]->setEndValue(QRect(300, 160, 50, 50));
            s_group->addAnimation(anim[0]);
        }
        if(score>=star_value*2)
        {
            star=2;
            star_label[1]->setGeometry(QRect(0,0,0,0));
            star_label[1]->show();
            anim[1]=new QPropertyAnimation(star_label[1], "geometry");
            anim[1]->setDuration(1000);
            anim[1]->setStartValue(QRect(335, 135, 100, 100));
            anim[1]->setEndValue(QRect(350, 160, 50, 50));
            s_group->addAnimation(anim[1]);
        }
        if(score>=star_value*3)
        {
            star=3;
            star_label[2]->setGeometry(QRect(0,0,0,0));
            star_label[2]->show();
            anim[2]=new QPropertyAnimation(star_label[2], "geometry");
            anim[2]->setDuration(1000);
            anim[2]->setStartValue(QRect(385, 135, 100, 100));
            anim[2]->setEndValue(QRect(400, 160, 50, 50));
            s_group->addAnimation(anim[2]);
        }
    }
    anim[3]=new QPropertyAnimation(g_label, "geometry");
    anim[3]->setDuration(1000);
    anim[3]->setStartValue(QRect(10, 190, 300, 80));
    anim[3]->setEndValue(QRect(10, 190, 300, 80));
    s_group->addAnimation(anim[3]);

    s_group->start();
    return;
}
void MainWindow::EndGame()
{
    if(!IsInEvent)
    {
        this->close();
        emit quit(star, score);
    }
    return;
}
bool MainWindow::IsRemove()//判斷盤面還有沒有消除
{
    for(int i=0; i<10; i++)
        for(int j=0; j<=7; j++)
            if(colorgroup(map[i][j])==colorgroup(map[i][j+1]) && colorgroup(map[i][j])==colorgroup(map[i][j+2]))
                return true;
    for(int j=0; j<10; j++)
        for(int i=0; i<=7; i++)
            if(colorgroup(map[i][j])==colorgroup(map[i+1][j]) && colorgroup(map[i][j])==colorgroup(map[i+2][j]))
                return true;
    return false;
}
bool MainWindow::IsSpecialMove(int color1, int color2)//判斷移動的糖果中有沒有特殊糖果
{
    if(color1==5||color2==5)
        return true;
    return false;
}
bool MainWindow::IsSpecialCreate()//判斷是否有待生成的特殊糖果
{
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
            if(Special[i][j])
                return true;
    return false;
}
bool MainWindow::IsSpecialRemove()//判斷是否有待消除的特殊糖果
{
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
            if(BombRemove[i][j] || VelimiRemove[i][j] || HelimiRemove[i][j])
                return true;
    return false;
}
bool MainWindow::IsStar(int color)//判斷是否產生星星
{
    for(int i=9; i>=4; i--)
        for(int j=0; j<10; j++)
            if(link[i][j])
                if(link[i-1][j] && link[i-2][j] && link[i-3][j] && link[i-4][j])
                {
                    if(drop_cnt==0)
                    {
                        if(color==colorgroup(map[s1/10][s1%10]).i)
                            Special[s1/10][s1%10]=17;
                        else if(color==colorgroup(map[s2/10][s2%10]).i)
                            Special[s2/10][s2%10]=17;
                    }
                    else
                        Special[i-2][j]=17;
                    return true;
                }
    for(int i=9; i>=0; i--)
        for(int j=0; j<=5; j++)
            if(link[i][j])
                if(link[i][j+1] && link[i][j+2] && link[i][j+3] && link[i][j+4])
                {
                    if(drop_cnt==0)
                    {
                        if(color==colorgroup(map[s1/10][s1%10]).i)
                            Special[s1/10][s1%10]=17;
                        else if(color==colorgroup(map[s2/10][s2%10]).i)
                            Special[s2/10][s2%10]=17;
                    }
                    else
                        Special[i][j+2]=17;
                    return true;
                }
    return false;
}
bool MainWindow::IsBomb(int color)//判斷是否產生炸彈
{
    for(int i=9; i>=2; i--)
        for(int j=0; j<=7; j++)
            if(link[i][j])
                if(link[i][j+1] && link[i][j+2] && link[i-1][j] && link[i-2][j])
                {
                    if(drop_cnt==0)
                    {
                        if(color==colorgroup(map[s1/10][s1%10]).i)
                            Special[s1/10][s1%10]=color+12;
                        else if(color==colorgroup(map[s2/10][s2%10]).i)
                            Special[s2/10][s2%10]=color+12;
                    }
                    else
                        Special[i][j]=color+12;
                    return true;
                }
    for(int i=9; i>=2; i--)
        for(int j=9; j>=2; j--)
            if(link[i][j])
                if(link[i][j-1] && link[i][j-2] && link[i-1][j] && link[i-2][j])
                {
                    if(drop_cnt==0)
                    {
                        if(color==colorgroup(map[s1/10][s1%10]).i)
                            Special[s1/10][s1%10]=color+12;
                        else if(color==colorgroup(map[s2/10][s2%10]).i)
                            Special[s2/10][s2%10]=color+12;
                    }
                    else
                        Special[i][j]=color+12;
                    return true;
                }
    for(int i=0; i<=7; i++)
        for(int j=0; j<=7; j++)
            if(link[i][j])
                if(link[i][j+1] && link[i][j+2] && link[i+1][j] && link[i+2][j])
                {
                    if(drop_cnt==0)
                    {
                        if(color==colorgroup(map[s1/10][s1%10]).i)
                            Special[s1/10][s1%10]=color+12;
                        else if(color==colorgroup(map[s2/10][s2%10]).i)
                            Special[s2/10][s2%10]=color+12;
                    }
                    else
                        Special[i][j]=color+12;
                    return true;
                }
    for(int i=0; i<=7; i++)
        for(int j=9; j>=2; j--)
            if(link[i][j])
                if(link[i][j-1] && link[i][j-2] && link[i+1][j] && link[i+2][j])
                {
                    if(drop_cnt==0)
                    {
                        if(color==colorgroup(map[s1/10][s1%10]).i)
                            Special[s1/10][s1%10]=color+12;
                        else if(color==colorgroup(map[s2/10][s2%10]).i)
                            Special[s2/10][s2%10]=color+12;
                    }
                    else
                        Special[i][j]=color+12;
                    return true;
                }
    return false;
}
bool MainWindow::IsVelimi(int color)//判斷是否產生垂直消除
{
    for(int i=9; i>=3; i--)
        for(int j=0; j<10; j++)
            if(link[i][j])
                if(link[i-1][j] && link[i-2][j] && link[i-3][j])
                {
                    if(drop_cnt==0)
                    {
                        if(color==colorgroup(map[s1/10][s1%10]).i)
                            Special[s1/10][s1%10]=color+4;
                        else if(color==colorgroup(map[s2/10][s2%10]).i)
                            Special[s2/10][s2%10]=color+4;
                    }
                    else
                        Special[i-1][j]=color+4;
                    return true;
                }
    return false;
}
bool MainWindow::IsHelimi(int color)//判斷是否產生水平消除
{
    for(int i=9; i>=0; i--)
        for(int j=0; j<=6; j++)
            if(link[i][j])
                if(link[i][j+1] && link[i][j+2] && link[i][j+3])
                {
                    if(drop_cnt==0)
                    {
                        if(color==colorgroup(map[s1/10][s1%10]).i)
                            Special[s1/10][s1%10]=color+8;
                        else if(color==colorgroup(map[s2/10][s2%10]).i)
                            Special[s2/10][s2%10]=color+8;
                    }
                    else
                        Special[i][j+1]=color+8;
                    return true;
                }
    return false;
}
MainWindow::~MainWindow()
{
    delete c_label;
    delete s_label;
    delete m_label;
    delete g_label;
    for(int i=0; i<100; i++)
    {
        delete back_label[i/10][i%10];
        delete button[i/10][i%10];
        delete anim[i];
    }
    for(int i=0; i<9; i++)
        delete p_group[i];
    for(int i=0; i<3; i++)
        delete star_label[i];
    delete signalMapper;
    delete g_anim;
    delete s_group;
    delete timer;
    delete ui;
}
