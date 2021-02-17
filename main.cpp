#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>

#include <netlistparsercpp.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    I_Parser* n = new NetlistParserCpp();
    n->parse("../NetlistCarver/netlistfiles/config.in");//path needs to come from a UI solution

    MainWindow w;
    w.show();

    return a.exec();
}
