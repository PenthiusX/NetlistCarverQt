#include "ReadDesign/netlistparsercpp.h"
#include <QRegularExpression>

NetlistParserBF::NetlistParserBF()
{

}

NetlistParserBF::~NetlistParserBF()
{
    for(auto ce : cells){delete ce;}
    lines.clear();
}

QString NetlistParserBF::ReadStringFromQrc(QString Filename)
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
    QByteArray qba = ReadStringFromQrc(path).toLocal8Bit();
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
    QRegExp rx3(R"(\w+=\w+\.\w+)");//word before and after includeing numbers with decimals.//fors Xcall
    QRegExp rx4(R"(/\s\w+)");//word after '/' for name of cellSbkt
    QRegExp rx5(R"([a-z0-9/<>_-]{1,20})");//encompasses a wide set with /, <> and _ undersore from 2 to 20 letters
    QRegExp rx6(R"((\w+)|([a-z0-9\/<>_-]{1,20}))");//
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
            while((pos = rx6.indexIn(lines[i],pos)) != -1)//parse over each word in the line
            {
                if(count == 1)
                {
                    tcell->name = rx6.cap(0);// store sbkt name
                }
                if(count >= 2)
                { // saves the port names
                    tcell->port.push_back(rx6.cap(0));
                }
                pos += rx6.matchedLength();//iterate pos over each word
                count++;
            }
        }

        //Hits when a line has MMos
        if(lines[i][0] == MMos)
        {
            int pos = 0;
            count = 0;

            ::MMos* tm = new ::MMos();
            while((pos = rx.indexIn(lines[i],pos)) != -1)//parse over each word in the line
            {
                if(count == 0)//First word
                {
                    tm->name = rx.cap(1);
                }
                if(count >= 1  && count <= 4)//first 4 items
                {
                    tm->pins.push_back(rx.cap(1));
                }
                pos += rx.matchedLength();//iterate pos over each word
                count++;
            }
            pos = 0;
            while((pos = rx2.indexIn(lines[i],pos)) != -1)//parse over the device properties
            {
                //for device properties
                QString ts = rx2.cap(1);
                ts.truncate(ts.lastIndexOf(QChar('=')));
                Device d;
                d.paramName = ts;
                d.paramValue = rx2.cap(1).remove(QRegularExpression("\\w+="));
                tm->deviceProperties.push_back(d);
                pos += rx2.matchedLength();//iterate pos over each word
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
            while((pos = rx5.indexIn(lines[i],pos)) != -1)
            {
                if(rx5.cap(0) == '/')//bool flag on '/' hit
                {
                    hitStop = true;
                }
                if(count == 0)//First word the name
                {
                    QString name = rx5.cap(0);
                    name.insert(0,'X');
                    xc->name = name;
                }
                if(!hitStop && count > 0)//all pins till it hits '/' in file ,and skips the first word that is the name.
                {
                    xc->pins.push_back(rx5.cap(0));
                }
                QString t = rx5.cap(0);
                pos += rx5.matchedLength();//iterate pos over each word
                count++;
            }
            pos = 0;
            while((pos = rx4.indexIn(lines[i],pos)) != -1)//capture the name of the Sbkt
            {
                QString name = rx4.cap(0);
                name.remove("/ ");
            // passes the real refrence to the object , so changes to the cell here will change the actual values.
                xc->cell = findCellFromName(name);
                pos += rx4.matchedLength();
            }
            pos = 0;
            while((pos = rx3.indexIn(lines[i],pos)) != -1)//capture the name of the Sbkt
            {
                QString t = rx3.cap(0);
                Device d;
                d.paramName = rx3.cap(0).remove(QRegularExpression("=\\w+\\.\\w+"));
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
                if(count > 0 )
                {
                    r->pins.push_back(rx6.cap(0));
                }
                if(count > 0 && count < 2)
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
                if(count > 0 && count < 2)
                {
                    c->pins.push_back(rx6.cap(0));
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
    return cells;
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
    }
    else{
        qInfo() << "THe cell array is empty cant find any such Object";
    }
}
