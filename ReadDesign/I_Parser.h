#pragma once

#include <QString>
#include <QDebug>
#include <QFile>

#include <vector>
#include <map>

#include <QRegExp>

#include "cellcbkt.h"

/*
 * Interface use only
 * No implementations will be defined here.
 * For interfacting with all Parser types and components.
 */

class I_Parser
{
public:
    virtual ~I_Parser(){}
    virtual  std::vector<CellCBKT*> parse(QString netlistFilePath) = 0;
};
