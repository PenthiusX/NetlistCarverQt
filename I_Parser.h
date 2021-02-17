#pragma once

#include <QString>
#include <QDebug>
#include <QFile>

#include <vector>
#include <map>

#include <QRegExp>

#include "cellcbkt.h"

/*
 * Interface work pending
 */
class CellCBKT;
class I_Parser
{
public:
    virtual ~I_Parser(){}
    virtual  std::vector<CellCBKT*> parse(QString netlistFilePath) = 0;
};
