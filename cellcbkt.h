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
struct MMos
{
    QString name;
    std::vector<QString> pins;
    std::vector<Device> deviceProperties;
};

struct Res {
    QString name;
    std::vector<QString> pins;//default 2
    unsigned int value;
};

struct Cap {
    QString name;
    std::vector<QString> pins;//default 2
    unsigned int value;
};

/*
  *
 */
class CellCBKT;

struct XCall {
    QString name;
    CellCBKT* cell;//real refrence of the initial obj needs to be stored.
    std::vector<QString> pins;
    std::vector<Device> deviceProperties; //eg lp=0.13u wp=8.6u wn=5.7u
};
/*
  *
 */
class CellCBKT {
public:
    CellCBKT(){};
    ~CellCBKT(){};

    QString name;
    std::vector<XCall*> xVec;
    std::vector<MMos*> mVec;
    //cap;
    //res;
    std::vector<QString> port;//pin names
//    std::vector<std::map<char,float>> deviceParams;
};
