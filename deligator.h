#ifndef DELIGATOR_H
#define DELIGATOR_H
#include <QString>
#include <vector>

#include "ReadDesign/cellcbkt.h"
#include "ReadConstraint/ReadConstraint.h"

class Deligator
{
public:
    Deligator();
    ~Deligator();

    void setPathforNetlistFile(QString s);
    void parseNetlist();
    void parseConstraint();
    void designRun();

private :

    QString netlistFilePath;;
    std::vector<CellCBKT*> tVector;
    CReadConstraints cr;

};

#endif // DELIGATOR_H
