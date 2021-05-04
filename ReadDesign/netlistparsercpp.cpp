#include "ReadDesign/netlistparsercpp.h"
#include <QRegularExpression>

#include <QDebug>
#include <QtDebug>

NetlistParserBF::NetlistParserBF()
{

}

NetlistParserBF::~NetlistParserBF()
{
    for(auto ce : cells){delete ce;}
    lines.clear();
}

QString NetlistParserBF::readStringFromQrc(QString Filename)
{
    // Read text files from .qrc file
    // return as QString
    QString mText = "";
    QFile mFile(Filename);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "could not open file for read";
    }
    QTextStream in(&mFile);
    mText = in.readAll();

    mFile.close();
    return mText;
}

/*
* ParseFIle function , Parses over netlist file and allocates the info into
* the designed containers.THe outuput will be presented in the objects returned
* in this case the 'cells' vector.
* Author : Aditya
*/
std::vector<CellCBKT*> NetlistParserBF::parse(QString path)
{
    qInfo() << "path" << path;
    QByteArray qba = readStringFromQrc(path).toLocal8Bit();
    QByteArray tb;//temp buffer

    //Stores the text from file into an array of strings
    //for each line.
    uint newLineTrip = 0;
    for(uint i = 0 ; i < qba.length(); i++)
    {
        tb.push_back(qba[i]);
        if(qba[i] == '\n' && qba[i+1] != '+' && newLineTrip == 0)
        {
            lines.push_back(QString::fromUtf8(tb));//push the line info into array
            tb.clear();//clears the buffer for new line
        }
        if(qba[i] == '+')//handels cases when data forthe line is moved to nextline folowing a +
        {
            newLineTrip++;
        }
        if(qba[i] == '\n' && newLineTrip >= 1 && qba[i+1] != '+')
        {
            lines.push_back(QString::fromUtf8(tb));//push the line info into array
            tb.clear();//clears the buffer for new line
            newLineTrip = 0;
        }
    }
    lines.push_back(".ENDS");//Handeling the last line exception

    //Some tokens---------
    char* start = ".SUBCKT ";
    char* end  = ".ENDS";
    char* MMos = "M";
    char* XCall = "X";
    char* ResCall = "R";
    char* CapacCall = "C";
    //Regexp captures-----
    QRegExp rx(R"((\w+))");//word of any length
    QRegExp rx2(R"((\w+=\w+))");//word before and after = // for Mmos
    QRegExp rx3(R"((\w+=\w+\.\w+)|(\w+=\w+\w+)|(\w+\=\d))");//word before and after includeing numbers with decimals.//fors Xcall
    QRegExp rx4(R"(/\s\w+)");//word after '/' for name of cellSbkt
    QRegExp rx5(R"([a-z0-9/<>_-]{1,20})");//encompasses a wide set with /, <> and _ undersore from 2 to 20 letters
    //QRegExp rx6(R"((\w+)|([a-z0-9\/<>_-]{1,20}))");//
    QRegExp rx6(R"((\w+[a-z0-9<>_-]{1,20})|(\w+)|(\/))");//
    QRegExp rx7(R"((\s\w\s))");
    //--------------------

    //Start parsing the lines.
    CellCBKT* tcell;
    for(uint i = 0; i < lines.size() ; i++)
    {
        lines[i].replace("\n","");//removes newline char command
        lines[i].replace('+',"");//remove continue line char command
        uint count;
        //Start of cell hits when it fine .SUBCKT in file.
        if(lines[i].contains(start))
        {
            int pos = 0;
            count = 0;
            tcell = new CellCBKT();//creates a new cell evertime it find a .SBKT in the linesss
            while((pos = rx6.indexIn(lines[i],pos)) != -1)
            {
                if(count == 1)
                {
                    tcell->name = rx6.cap(0);// store sbkt name
                }
                if(count >= 2)
                { // saves the port names
                    tcell->ports.push_back(rx6.cap(0));
                }
                pos += rx6.matchedLength();
                count++;
            }
        }

        //Hits when a line has MMos
        if(lines[i][0] == MMos)
        {
            int pos = 0;
            count = 0;

            ::MMos* tm = new ::MMos();
            while((pos = rx.indexIn(lines[i],pos)) != -1)
            {
                if(count == 0)//First word
                {
                    tm->name = rx.cap(1);
                }
                if(count >= 1  && count <= 4)//first 4 items
                {
                    tm->ports.push_back(rx.cap(1));
                }
                pos += rx.matchedLength();
                count++;
            }
            pos = 0;
            while((pos = rx3.indexIn(lines[i],pos)) != -1)
            {
                QString t = rx3.cap(0);
                Device d;
                d.paramName = rx3.cap(0).remove(QRegularExpression(R"("(=\w+\.\w+)|(=\w+\w+)|(=\d))"));
                d.paramValue = rx3.cap(0).remove(QRegularExpression("\\w+="));
                tm->deviceProperties.push_back(d);//add all device and values for the current Xcall;
                pos += rx3.matchedLength();
            }
            pos = 0;
            while((pos = rx7.indexIn(lines[i],pos)) != -1)
            {
                QString t7 = rx7.cap(0);
                t7.remove(" ");
                if(t7 == "P"){
                    tm->type = t7;
                }
                if(t7 == "N"){
                    tm->type = t7;
                }
                pos += rx7.matchedLength();
            }

            tcell->mVec.push_back(tm);//store the Mmos info
        }

        //Runs after all cells have been defined , Xcall will call only a defined cell
        if(lines[i][0] == XCall)
        {
            int pos = 0;
            count = 0;
            ::XCall* xc = new ::XCall();
            bool hitStop = false;
            while((pos = rx6.indexIn(lines[i],pos)) != -1)
            {
                if(rx6.cap(0) == '/')//bool flag on '/' hit #Note : Need to add another condition for it as well witout slash.
                {
                    hitStop = true;
                }
                if(count == 0)//First word the name
                {
                    QString name = rx6.cap(0);
                    //                    name.insert(0,'X');
                    xc->name = name;
                }
                if(hitStop == false && count > 0)//all pins till it hits '/' in file ,and skips the first word that is the name.
                {
                    xc->ports.push_back(rx6.cap(0));
                }
                QString t = rx6.cap(0);
                pos += rx6.matchedLength();//iterate pos over each word
                count++;
            }
            pos = 0;
            if(lines[i].contains("/")){
                while((pos = rx4.indexIn(lines[i],pos)) != -1)//capture the name of the Sbkt
                {
                    QString name = rx4.cap(0);
                    name.remove("/ ");
                    // passes the real refrence to the object , so changes to the cell here will change the actual values.
                    xc->cell = findCellFromName(name);
                    pos += rx4.matchedLength();
                }
            }
            if(!lines[i].contains("/")){
                QString name;
                while((pos = rx6.indexIn(lines[i],pos)) != -1)//capture the name of the Sbkt
                {
                    name = rx6.cap(0);
                    pos += rx6.matchedLength();
                }
                xc->cell = findCellFromName(name);
            }

            pos = 0;
            while((pos = rx3.indexIn(lines[i],pos)) != -1)//capture the device and its property value
            {
                QString t = rx3.cap(0);
                Device d;
                d.paramName = rx3.cap(0).remove(QRegularExpression("(=\\w+\\.\\w+)|(=\\w+\\w+)"));
                d.paramValue = rx3.cap(0).remove(QRegularExpression("\\w+="));
                xc->deviceProperties.push_back(d);//add all device and values for the current Xcall;
                pos += rx3.matchedLength();
            }
            tcell->xVec.push_back(xc);
        }

        //Hits when a line has Resistor
        if(lines[i][0] == ResCall)
        {
            int pos = 0;
            count = 0;
            Res* r = new Res();
            while((pos = rx6.indexIn(lines[i],pos)) != -1)//parse over each word in the line
            {
                if(count == 0)
                {
                    r->name = rx6.cap(0);// store sbkt name
                }
                if(count > 0 && count <= 2)
                {
                    r->ports.push_back(rx6.cap(0));
                }
                if(count > 2)
                {
                    r->value = rx6.cap(0).toUInt();
                }
                pos += rx6.matchedLength();//iterate pos over each word
                count++;
            }
            tcell->rVec.push_back(r);
        }

        //Hits when a line has Capacitor
        if(lines[i][0] == CapacCall)
        {
            int pos = 0;
            count = 0;
            Cap* c = new Cap();
            while((pos = rx6.indexIn(lines[i],pos)) != -1)//parse over each word in the line
            {
                if(count == 0)
                {
                    c->name = rx6.cap(0);// store sbkt name
                }
                if(count > 0 && count <= 2)
                {
                    c->ports.push_back(rx6.cap(0));
                }
                if(count > 2)
                {
                    c->value = rx6.cap(0).toUInt();
                }
                pos += rx6.matchedLength();//iterate pos over each word
                count++;
            }
            tcell->cVec.push_back(c);
        }

        //When reaches .END of the spkt module
        if(lines[i].contains(end))
        {//Push the sbkt cell module populated above,into the cells array.
            cells.push_back(tcell);
            tcell = NULL;//clear the cell for use again for nex cell info.
        }
    }

    cells.pop_back();//Temporary fix, for the extra empty array element created, due to bug, needs fix.
    return cells;
}
/*
 * Takes in Parameters for the port names from Xcall and the relvant subcell,
 * compares the names and creates relvant match associations base on the order
 * wich the ports were introduces to the arrays.
 * Returns a vetor of Device type objects with hold a set of two string
 * Author: Aditya Mattoo
 */
std::vector<Device> NetlistParserBF::getPortNameMatchArray(std::vector<QString> xc ,std::vector<QString> mcr){

    std::vector<Device> d;

    for(uint i = 0 ; i < xc.size() ;i++){
        Device td;
        td.paramName = mcr[i];
        td.paramValue = xc[i];
        d.push_back(td);
    }

    return d;
}
/*
 * Takes in Parameters for the port names from Xcall and the relvant subcell,
 * compares the names and creates relvant match associations base on the order
 * wich the ports were introduces to the arrays.
 * Returns a map with key value pairs.
 * Author: Aditya Mattoo
 */
std::map<QString,QString> NetlistParserBF::getPortNameMatchMap(std::vector<QString> xc ,std::vector<QString> mcr){

    std::map<QString,QString> compMap;

    for(uint i = 0 ; i < xc.size() ;i++){
        compMap.insert(std::pair<QString,QString>(mcr[i],xc[i]));
    }
    return compMap;
}
/*
 * Find the relative port from the precreated associative Array above in the getPortNameMatchArray()
 * function to give you the correct port name that was associated with it in the Xcall\Cell component
* Author: Aditya Mattoo
*/
QString NetlistParserBF::findRelativePort(QString po,std::vector<Device> arr,QString Xcallname){
    for(uint i = 0 ; i < arr.size() ; i++){
        if(po == arr[i].paramName){
            return arr[i].paramValue;
        }
    }
    return Xcallname + "/" + po;//if name does not match then add it as an internal conntection
}

Device NetlistParserBF::comapreAndRevertDeviceValue(Device cellMos,std::vector<Device> XcallDevices){

    Device rd;
    for(uint n = 0 ; n < XcallDevices.size() ; n++){
        if(cellMos.paramValue == XcallDevices[n].paramName){
            rd.paramName = cellMos.paramName;
            rd.paramValue = XcallDevices[n].paramValue;
            return rd;
        }
    }
    return cellMos;
}
/*
 * Temp implementation to dump the info as Application output
 * Author:Aditya Mattoo
 */
void NetlistParserBF::netlistDump(std::vector<CellCBKT *> locVec)
{
    for(uint l = 0; l < locVec.size(); l++)
    {
        qInfo() << " ";
        qInfo() << "--------------------------------------------------";
        qInfo() << " ";
        QString s;
        s += locVec[l]->name;
        for(uint p = 0 ; p < locVec[l]->ports.size(); p++)
        {
            s += " " + locVec[l]->ports[p];
        }
        qInfo() << s;
        s.clear();
        for(uint m = 0; m < locVec[l]->mVec.size(); m++)
        {
            s += locVec[l]->mVec[m]->name +"|" ;
            for(uint p2 = 0 ; p2 < locVec[l]->mVec[m]->ports.size(); p2++)
            {
                s += " (" + locVec[l]->mVec[m]->ports[p2] + ")";
            }
            for(uint d = 0 ; d < locVec[l]->mVec[m]->deviceProperties.size(); d++)
            {
                s += " " + locVec[l]->mVec[m]->deviceProperties[d].paramName + ":" + locVec[l]->mVec[m]->deviceProperties[d].paramValue;
            }
            qInfo() << s;
            s.clear();
        }
        qInfo() << " ";
        qInfo() << "--------------------------------------------------";
        qInfo() << " ";
        s.clear();
    }
}
/*
 *
 *
*/
std::vector<CellCBKT *> NetlistParserBF::parse(QString path, char hint)
{
    //start flatenign if hint = 'F'
    if(hint == 'F')
    {
        std::vector<CellCBKT *> locVec;
        locVec = this->parse(path);

        std::vector<Device> dt;//comparitve array for ports.
        for(uint n = 0; n < locVec.size(); n++)
        {
            if(locVec[n] != nullptr && locVec[n]->xVec.size() != 0)
            {
                qInfo() << "Flatenning started" << locVec[n]->name;//Post this message on UI
                for(uint x = 0 ; x < locVec[n]->xVec.size() ; x++)//for every Xcall
                {
                    //FOr each xcall push the relvant mos /res/cap variant in existant vectors
                    if(locVec[n]->xVec[x]->cell == NULL){
                        qInfo() << "Problem in file , Check if Xcall has definition for its cell." + locVec[n]->xVec[x]->cell->name;//Post this message on UI
                    }
                    if(locVec[n]->xVec[x]->ports.size() == locVec[n]->xVec[x]->cell->ports.size())
                    {//number ports for xcall and its relvant cell need to be the same
                        dt = getPortNameMatchArray(locVec[n]->xVec[x]->ports,locVec[n]->xVec[x]->cell->ports);//populate
                    }
                    else
                    {
                        qInfo() << "Port count mismatch please check if netlist file is correct.";//Post this message on UI
                    }

                    MMos* tm;
                    for(uint m = 0 ; m < locVec[n]->xVec[x]->cell->mVec.size() ; m++)//For every Mmos in Xcall cell.
                    {
                        tm = new MMos;
                        tm->name = locVec[n]->xVec[x]->name + "/" + locVec[n]->xVec[x]->cell->mVec[m]->name;//set name
                        tm->type =  locVec[n]->xVec[x]->cell->mVec[m]->type;

                        for(uint p = 0 ; p < locVec[n]->xVec[x]->cell->mVec[m]->ports.size(); p++)//set port names
                        {
                            tm->ports.push_back(findRelativePort(locVec[n]->xVec[x]->cell->mVec[m]->ports[p],dt,locVec[n]->xVec[x]->name));
                        }
                        for(uint d = 0 ; d < locVec[n]->xVec[x]->cell->mVec[m]->deviceProperties.size(); d++)
                        {
                            Device td;
                            td = comapreAndRevertDeviceValue(locVec[n]->xVec[x]->cell->mVec[m]->deviceProperties[d],locVec[n]->xVec[x]->deviceProperties);
                            tm->deviceProperties.push_back(td);
                        }

                        locVec[n]->mVec.push_back(tm);
                        tm = NULL;
                    }
                    Res* rm;//For every Resistor encoutered
                    for(uint r = 0 ; r < locVec[n]->xVec[x]->cell->rVec.size() ; r++)
                    {
                        rm = new Res();
                        rm->name = locVec[n]->xVec[x]->name + "/" + locVec[n]->xVec[x]->cell->rVec[r]->name;
                        rm->value = locVec[n]->xVec[x]->cell->rVec[r]->value;

                        for(uint p = 0 ; p < locVec[n]->xVec[x]->cell->rVec[r]->ports.size(); p++)
                        {
                            rm->ports.push_back(findRelativePort(locVec[n]->xVec[x]->cell->rVec[r]->ports[p],dt,locVec[n]->xVec[x]->name));
                        }

                        locVec[n]->rVec.push_back(rm);
                        rm = NULL;
                    }
                    Cap* cm;//For every Cpacitor encountered
                    for(uint c = 0 ; c < locVec[n]->xVec[x]->cell->cVec.size() ; c++)
                    {
                        cm = new Cap();
                        cm->name = locVec[n]->xVec[x]->name + "/" + locVec[n]->xVec[x]->cell->cVec[c]->name;
                        cm->value = locVec[n]->xVec[x]->cell->cVec[c]->value;

                        for(uint p = 0 ; p < locVec[n]->xVec[x]->cell->cVec[c]->ports.size(); p++)
                        {
                            cm->ports.push_back(findRelativePort(locVec[n]->xVec[x]->cell->cVec[c]->ports[p],dt,locVec[n]->xVec[x]->name));
                        }

                        locVec[n]->cVec.push_back(cm);
                        cm = NULL;
                    }
                }
                locVec[n]->xVec.clear();//optional clear after flatening the Xcall to its sub m/c/r moses
            }
            else
            {
                if(locVec[n] != nullptr)
                    qInfo() << "No Xcall found to flatten in" << locVec[n]->name; //Post this message on UI
                else
                    qInfo() << "No cell oject found"; //Post this message on UI
            }
        }

        netlistDump(locVec);//Test dump
        return locVec;
    }
    if(hint == 'R')
    {
        std::vector<CellCBKT *> locVec;
        return this->parse(path);
    }

    qInfo() << "Please enter the correct hint."; //Post this message on UI
}
/*
 * Used to return the refrence of the cell for use in a Xcall container
 * Must have the cells prepopulated.
*/
CellCBKT* NetlistParserBF::findCellFromName(QString name)
{
    if(cells.size() != 0){
        for(uint i = 0 ; i< cells.size() ; i++){
            if(cells[i]->name == name){
                return cells[i];
            }
        }
        qInfo() << "no Cell with the matching name found";
        CellCBKT* badCell = NULL;
        return badCell;
    }
    else{
        qInfo() << "THe cell array is empty cant find any such Object";
    }
}

