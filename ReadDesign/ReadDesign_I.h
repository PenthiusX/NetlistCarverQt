#pragma once

#include <vector>
#include <QString>


#include "cellcbkt.h"
#include "ReadConstraint/ReadConstraint.h"

class ReadDesign_I
{
public:
    virtual ~ReadDesign_I(){}
    //Parse the Container and CreadContraint container and post out the
    //traversed and flipped flattned netlist info.
    virtual void setStates(std::vector<CellCBKT*> cv , CReadConstraints c ) = 0;
};
