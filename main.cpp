
#include <QApplication>
#include <stdio.h>
#include <vector>

#include "mainwindow.h"
#include "ReadDesign/netlistparsercpp.h"
#include "ReadDesign/cellcbkt.h"
#include "ReadConstraint/ReadConstraint.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    //Read constraint instance//
     std::string readConstraintInputFile = "../NetlistCarverQt/netlistfiles/readConstraint_new1.xml";
     CReadConstraints obj;
     CReadConstraints c = obj.parseLoadConstraintFile(readConstraintInputFile);

    //--Netlist parser Instansce--//
    NetlistParser_I* n = new NetlistParserBF();
    std::vector<CellCBKT*> tVector = n->parse("../NetlistCarverQt/netlistfiles/4x4x64x2.cdl",'F');//path needs to come from a UI solution
    //-------------------------//

    MainWindow w;
    w.show();

    return a.exec();
}
