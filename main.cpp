#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>

#include <netlistparsercpp.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NetlistParserCpp n;
    n.parseFile("../NetlistCarver/netlistfiles/config.in");//path needs to come from a UI solution


    MainWindow w;
    w.show();


    return a.exec();
}
