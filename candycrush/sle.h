#ifndef SLE
#define SLE

#include "mainwindow.h"
class sle : public MainWindow{
public slots:
    void start()
    {
        s_eli_start();
    }
};

#endif // SLE

