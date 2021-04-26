#include "readdesign.h"
#include <QDebug>


ReadDesign::ReadDesign()
{

}

ReadDesign::~ReadDesign()
{

}

void ReadDesign::setStates(std::vector<CellCBKT *> cv, CReadConstraints cr)
{
    uint count1 = 0,count2 = 0;
    for(uint c = 0 ; c < cr.setHigh[0].nets.size(); c++)
    {
        //search for name.
        for(uint i = 0 ; i < cv[cv.size()-1]->mVec.size() ; i++)
        {
            for(uint p = 0 ; p < cv[cv.size()-1]->mVec[i]->ports.size() ; p++)
            {
                QString s1 = cv[cv.size()-1]->mVec[i]->ports[p];
                QString s2 = cr.setHigh[0].nets[c].c_str();
                if(s1 == s2)//compare the ports.
                {
                      qInfo() << cv[cv.size()-1]->mVec[i]->ports[p] << "---" << cr.setHigh[0].nets[c].c_str();
                      if(p == 0){
                         count1++;//output
                      }
                      if(p == 1){
                          count2++;//input
                      }
                }
            }
        }
    }

    for(uint l = 0; l < cv.size(); l++)
    {
        QString s;
        s += cv[l]->name;
        for(uint p = 0 ; p < cv[l]->ports.size(); p++)
        {
            s += " " + cv[l]->ports[p];
        }
        qInfo() << s;
        s.clear();
        for(uint m = 0; m < cv[l]->mVec.size(); m++)
        {
            s += cv[l]->mVec[m]->name +"|" ;
            for(uint p2 = 0 ; p2 < cv[l]->mVec[m]->ports.size(); p2++)
            {
                s += " (" + cv[l]->mVec[m]->ports[p2] + ")";
            }
            for(uint d = 0 ; d < cv[l]->mVec[m]->deviceProperties.size(); d++)
            {
                s += " " + cv[l]->mVec[m]->deviceProperties[d].paramName + ":" + cv[l]->mVec[m]->deviceProperties[d].paramValue;
            }
            qInfo() << s;
            s.clear();
        }
        s.clear();
    }
}
