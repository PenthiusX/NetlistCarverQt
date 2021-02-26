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
struct MMos//Mmos
{
    QString name;
    std::vector<QString> pins;
    std::vector<Device> deviceProperties;
};

struct Res {//Resistor
    QString name;
    std::vector<QString> pins;
    unsigned int value;
};

struct Cap {//Capacitor
    QString name;
    std::vector<QString> pins;
    unsigned int value;
};

/*
  *
 */
class CellCBKT;

struct XCall { //Xcall container
    QString name;
    CellCBKT* cell;//real refrence of the initial obj needs to be stored.
    std::vector<QString> pins;
    std::vector<Device> deviceProperties; //eg lp=0.13u wp=8.6u wn=5.7u
};
/*
  *
 */
class CellCBKT {//Cell container
public:
    CellCBKT(){};
    ~CellCBKT(){};

    QString name;
    std::vector<XCall*> xVec;//d
    std::vector<MMos*> mVec;//d
    std::vector<Res*> rVec;//p
    std::vector<Cap*> cVec;//p
    std::vector<QString> port;//pin names
//    std::vector<std::map<char,float>> deviceParams;
};
