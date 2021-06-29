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
    this->lcv = cv;
    this->lcr = cr;

    for(uint c = 0 ; c < cr.setHigh[0].nets.size(); c++)
    {   //1 to set high
        propegate(cr.setHigh[0].nets[c].c_str(),"1");//setting high

        //        //search for name.
        //        for(uint i = 0 ; i < cv[cv.size()-1]->mVec.size() ; i++)
        //        {
        //            for(uint p = 0 ; p < cv[cv.size()-1]->mVec[i]->ports.size() ; p++)
        //            {
        //                QString s1 = cv[cv.size()-1]->mVec[i]->ports[p];
        //                QString type = cv[cv.size()-1]->mVec[i]->type;
        //                QString s2 = cr.setHigh[0].nets[c].c_str();
        //                if(s1 == s2 &&  type == "P")//compare the ports.
        //                {
        //                    if(p == 0)//output
        //                    {
        //                        cv[cv.size()-1]->mVec[i]->ports[p] = cv[cv.size()-1]->mVec[i]->ports[p] + "[1]";
        //                        //for set high on p type no change
        //                    }
        //                    if(p == 1)//input
        //                    {
        //                        cv[cv.size()-1]->mVec[i]->ports[p] = cv[cv.size()-1]->mVec[i]->ports[p] + "[1]";
        //                        //default or no change
        //                        cv[cv.size()-1]->mVec[i]->ports[p-1] = cv[cv.size()-1]->mVec[i]->ports[p-1] + "[*]";
        //                    }
        //                }
        //                if(s1 == s2 &&  cv[cv.size()-1]->mVec[i]->type == "N")//compare the ports.
        //                {
        //                    if(p == 0)//output
        //                    {
        //                        cv[cv.size()-1]->mVec[i]->ports[p] = cv[cv.size()-1]->mVec[i]->ports[p] + "[1]";

        //                    }
        //                    if(p == 1)//input
        //                    {
        //                        cv[cv.size()-1]->mVec[i]->ports[p] = cv[cv.size()-1]->mVec[i]->ports[p] + "[1]";
        //                        //for N type is value is 1 out put is 0
        //                        cv[cv.size()-1]->mVec[i]->ports[p-1] = cv[cv.size()-1]->mVec[i]->ports[p-1] + "[0]";
        //                    }
        //                }
        //            }
        //        }
    }

    //Print the info
    for(uint l = lcv.size()-1; l < lcv.size(); l++)
    {
        QString s;
        s += lcv[l]->name;
        for(uint p = 0 ; p < lcv[l]->ports.size(); p++)
        {
            s += " " + lcv[l]->ports[p];
        }
        qInfo() << s;
        s.clear();
        for(uint m = 0; m < lcv[l]->mVec.size(); m++)
        {
            s += lcv[l]->mVec[m]->name +"|" ;
            for(uint p2 = 0 ; p2 < lcv[l]->mVec[m]->ports.size(); p2++)
            {
                s += " (" + lcv[l]->mVec[m]->ports[p2] + ")";
            }
            for(uint d = 0 ; d < lcv[l]->mVec[m]->deviceProperties.size(); d++)
            {
                s += " " + lcv[l]->mVec[m]->deviceProperties[d].paramName + ":" + lcv[l]->mVec[m]->deviceProperties[d].paramValue;
            }
            qInfo() << s;
            s.clear();
        }
        s.clear();
    }
}

void ReadDesign::propegate(QString port,QString state)//runs recursively
{
    //search for name.
    for(uint i = 0 ; i < lcv[lcv.size()-1]->mVec.size() ; i++)
    {
        for(uint p = 0 ; p < lcv[lcv.size()-1]->mVec[i]->ports.size() ; p++)
        {
            QString s1 = lcv[lcv.size()-1]->mVec[i]->ports[p];
            QString s2 = port;//the port flip
            QString type = lcv[lcv.size()-1]->mVec[i]->type;

            if(s1 == s2 &&  type == "P")//compare the ports.
            {
                if(p == 0)//output/drain
                {
                    lcv[lcv.size()-1]->mVec[i]->ports[p] = lcv[lcv.size()-1]->mVec[i]->ports[p] + "["+state+"]";
                    //for set high on p type no change  
                }
                if(p == 1)//input/gate
                {
                    lcv[lcv.size()-1]->mVec[i]->ports[p] = lcv[lcv.size()-1]->mVec[i]->ports[p] + "["+state+"]";
                    if(state == "1")//high
                    {
//                      lcv[lcv.size()-1]->mVec[i]->ports[p-1] = lcv[lcv.size()-1]->mVec[i]->ports[p-1] + "[*]";
                        propegate(lcv[lcv.size()-1]->mVec[i]->ports[p-1],"*");
                    }
                    if(state == "0")//low
                    {
                       // lcv[lcv.size()-1]->mVec[i]->ports[p-1] = lcv[lcv.size()-1]->mVec[i]->ports[p-1] + "[1]";
                        propegate(lcv[lcv.size()-1]->mVec[i]->ports[p-1],"1");
                    }
                }
                if(p == 2)//source
                {
                    lcv[lcv.size()-1]->mVec[i]->ports[p] = lcv[lcv.size()-1]->mVec[i]->ports[p] + "["+state+"]";
                    //check what the status of the Gate/input is
                }
            }
            if(s1 == s2 &&  type == "N")//compare the ports.
            {
                if(p == 0)//output/drain
                {
                    lcv[lcv.size()-1]->mVec[i]->ports[p] = lcv[lcv.size()-1]->mVec[i]->ports[p] + "["+state+"]";
                }
                if(p == 1)//input/gate
                {
                    lcv[lcv.size()-1]->mVec[i]->ports[p] = lcv[lcv.size()-1]->mVec[i]->ports[p] + "["+state+"]";
                    if(state == "1")//high
                    {
                        propegate(lcv[lcv.size()-1]->mVec[i]->ports[p-1],"0");
                    }
                    if(state == "0")//low
                    {
                        propegate(lcv[lcv.size()-1]->mVec[i]->ports[p-1],"*");
                    }
                }
                if(p == 2)//source
                {
                    lcv[lcv.size()-1]->mVec[i]->ports[p] = lcv[lcv.size()-1]->mVec[i]->ports[p] + "["+state+"]";
                    //check what the status of the Gate/input is
                }
            }
        }
    }
}
