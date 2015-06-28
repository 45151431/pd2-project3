#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
    t=new tle;
    s=new sle;
    connect(ui->s_game, SIGNAL(clicked()), this, SLOT(s_game()));
    connect(ui->t_game, SIGNAL(clicked()), this, SLOT(t_game()));
    connect(s, SIGNAL(quit(int, int)), this, SLOT(Menu(int, int)));
    connect(t, SIGNAL(quit(int, int)), this, SLOT(Menu(int, int)));
}

menu::~menu()
{
    emit quit(star ,score);
    delete t;
    delete s;
    delete ui;
}
