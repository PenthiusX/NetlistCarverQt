#pragma once

#include "ReadDesign_I.h"

class ReadDesign : public ReadDesign_I
{
public:
    ReadDesign();
    virtual ~ReadDesign();

    void setStates(std::vector<CellCBKT*> cv , CReadConstraints c );

private:
    void propegate(QString port,QString state);//state : 0 = low , 1=high ,2 = nochange/default
    std::vector<CellCBKT*> lcv; //local cell vector
    CReadConstraints lcr; // local constraint file
};
