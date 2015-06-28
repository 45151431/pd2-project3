#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <QPushButton>
#include "sle.h"
#include "tle.h"
#include <QDebug>

namespace Ui {
class menu;
}

class menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = 0);
    ~menu();

public slots:
    void Menu(int _star, int _score)
    {
        this->show();
        star=_star;
        score=_score;
    }
    void s_game()
    {
        this->close();
        s->show();
        s->game_init();
    }
    void t_game()
    {
        this->close();
        t->show();
        t->game_init();
    }
    void f_game()
    {
        emit quit(star, score);
        this->close();
    }

signals:
    void quit(int star, int score);

private:
    Ui::menu *ui;
    MainWindow *s, *t;
    int star, score;
};

#endif // MENU_H
