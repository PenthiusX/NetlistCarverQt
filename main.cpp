#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>

#include <netlistparsercpp.h>
#include <cellcbkt.h>
#include <vector>

#include <ReadConstraint/ReadConstraint.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Read constraint instance//
     std::string readConstraintInputFile = "../NetlistCarverQt/netlistfiles/readConstraint_new1.xml" ;
     CReadConstraints obj;
     CReadConstraints c = obj.parseLoadConstraintFile( readConstraintInputFile );

    //--Netlist parser Instansce--//
    I_Parser* n = new NetlistParserBF();
    std::vector<CellCBKT*> t = n->parse("../NetlistCarverQt/netlistfiles/config.in");//path needs to come from a UI solution
    //-------------------------//

    MainWindow w;
    w.show();

    return a.exec();
}
