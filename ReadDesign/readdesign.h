#pragma once

#include "ReadDesign_I.h"

class ReadDesign : public ReadDesign_I
{
public:
    ReadDesign();
    virtual ~ReadDesign();

    void setStates(std::vector<CellCBKT*> cv , CReadConstraints c );


};
