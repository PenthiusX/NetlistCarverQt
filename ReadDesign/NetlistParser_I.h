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

class NetlistParser_I
{
public:
    virtual ~NetlistParser_I(){}
    virtual  std::vector<CellCBKT*> parse(QString netlistFilePath) = 0;
    virtual std::vector<CellCBKT*> parse(QString netlistFilePath,char hint) = 0;
};
