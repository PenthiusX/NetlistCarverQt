#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>

#include <netlistparsercpp.h>
#include <cellcbkt.h>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //--This will move to the--//
    I_Parser* n = new NetlistParserBF();
    std::vector<CellCBKT*> t = n->parse("../NetlistCarverQt/netlistfiles/config.in");//path needs to come from a UI solution
    //-------------------------//

    MainWindow w;
    w.show();

    return a.exec();
}
