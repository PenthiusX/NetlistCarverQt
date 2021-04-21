#include "deligator.h"
#include "ReadDesign/NetlistParser_I.h"
#include "ReadDesign/netlistparsercpp.h"

#include "ReadDesign/ReadDesign_I.h"
#include "ReadDesign/readdesign.h"

Deligator::Deligator()
{

}

Deligator::~Deligator()
{

}

void Deligator::setPathforNetlistFile(QString s)
{
  this->netlistFilePath = s;
}

void Deligator::parseNetlist()
{
    if(netlistFilePath.size() != 0)
    {
        //--Netlist parser Instansce--//
        NetlistParser_I* n = new NetlistParserBF();
        this->tVector = n->parse(this->netlistFilePath,'F');//path needs to come from a UI solution
    }
    else{
        qInfo() << "Nelist file is empty or incorrect";
    }
}

void Deligator::parseConstraint()
{
    std::string readConstraintInputFile = "../NetlistCarverQt/netlistfiles/constraintFile1.xml";
    CReadConstraints obj;
    this->cr = obj.parseLoadConstraintFile(readConstraintInputFile);
}

void Deligator::designRun()
{
    parseNetlist();
    parseConstraint();
    ReadDesign_I *r = new ReadDesign();
    r->setStates(this->tVector,this->cr);
}
