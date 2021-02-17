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
public:
    MMos(){};
    ~MMos(){};
//    void setName(QString s);
//    QString getName();
//    void setPins(std::vector<QString> pins);
//    std::vector<QString> getPins();
//    void setDevices(std::vector<Device> d);
//    std::vector<Device> getDevice();
    
//private:
    QString name;
    std::vector<QString> pins;
    std::vector<Device> devices;
};
/*
  *
 */
class CellCBKT;

class XMos {
public:
    XMos(){};
    ~XMos(){};

//    void setName(QString n);
//    QString getName();
//    void setPins(std::vector<QString> pins);
//    std::vector<QString> getPins();
//    //
//    std::vector<std::map<char,float>> deviceParams;//Pending getset

//private:
    QString name;
    CellCBKT* cell;
    std::vector<QString> pins;

};
/*
  *
 */
class CellCBKT {
public:
    CellCBKT(){};
    ~CellCBKT(){};

//    void setName(QString n);
//    QString getName();
//    void setXMosVec(std::vector<XMos*> xvec);
//    std::vector<XMos*> getXMosVec();
//    void setMMosVec(std::vector<MMos*> mvec);
//    std::vector<MMos*> getMMosVec();

//private:
    QString name;
    std::vector<XMos*> xVec;
    std::vector<MMos*> mVec;
    std::vector<QString> port;//pin names
    std::vector<std::map<char,float>> deviceParams;
};
