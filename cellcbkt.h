#pragma once

#include <qstring.h>
#include <vector>
#include <map>

struct Device {
public:
    QString paramName;
    QString paramValue;
};
/*
  *
 */
class MMos
{
    QString name;
    std::vector<QString> pins;
    std::vector<Device> devices;
};
/*
  *
 */
class CellCBKT;

class XMos {
    QString name;
    CellCBKT* cell;
    std::vector<QString> pins;
    std::vector<std::map<char,float>> deviceParams;
};
/*
  *
 */
class CellCBKT {
public:
    QString name;
    std::vector<XMos*> x;
    std::vector<MMos*> m;
    std::vector<QString> port;//pin names
    std::vector<std::map<char,float>> deviceParams;
};
