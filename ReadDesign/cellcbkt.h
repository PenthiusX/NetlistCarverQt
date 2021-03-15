#pragma once

#include <qstring.h>
#include <vector>
#include <map>

struct Device {//used for keeping two string values that have some association.
public:        //Can be param and value or Pin association between Mos and Xcall.
    QString paramName;
    QString paramValue;
};
/*
  *
 */
struct MMos//Mmos
{
    QString name;
    std::vector<QString> ports;
    std::vector<Device> deviceProperties;
};

struct Res {//Resistor
    QString name;
    std::vector<QString> ports;
    unsigned int value;
};

struct Cap {//Capacitor
    QString name;
    std::vector<QString> ports;
    unsigned int value;
};

/*
  *
 */
class CellCBKT;

struct XCall { //Xcall container
    QString name;
    CellCBKT* cell;//real refrence of the initial obj needs to be stored.
    std::vector<QString> ports;
    std::vector<Device> deviceProperties;//eg lp=0.13u wp=8.6u wn=5.7u
};
/*
  *
 */
class CellCBKT {//Cell container
public:
    CellCBKT(){};
    CellCBKT(const CellCBKT &cpyc){//copy costructor
        QString name = cpyc.name;
        std::vector<XCall*> xVec = cpyc.xVec;//d
        std::vector<MMos*> mVec = cpyc.mVec;//d
        std::vector<Res*> rVec = cpyc.rVec;//p
        std::vector<Cap*> cVec = cpyc.cVec;//p
        std::vector<QString> ports = cpyc.ports;//pin names
    };
    ~CellCBKT(){};

    QString name;
    std::vector<XCall*> xVec;//d
    std::vector<MMos*> mVec;//d
    std::vector<Res*> rVec;//p
    std::vector<Cap*> cVec;//p
    std::vector<QString> ports;//pin names
//    std::vector<std::map<char,float>> deviceParams;
};
