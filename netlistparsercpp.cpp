#include "netlistparsercpp.h"

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
* ParseFIle function , Parses over netlist file and allocated the info into
* the designed containers.
* Author : Aditya
*/
std::vector<CellCBKT*> NetlistParserBF::parse(QString path)
{
    qInfo() << "path" << path;
    QByteArray qba = ReadStringFromQrc(path).toLocal8Bit();
    QByteArray tb;//temp buffer

    uint newLineTrip = 0;
    for(uint i = 0 ; i < qba.length(); i++)
    {
        tb.push_back(qba[i]);
        if(qba[i] == '\n' && qba[i+1] != '+' && newLineTrip == 0)
        {
            lines.push_back(QString::fromUtf8(tb));//push the line info into array
            tb.clear();//clears the buffer for new line
        }
        if(qba[i] == '+'){
            newLineTrip++;
        }
        if(qba[i] == '\n' && newLineTrip >= 1 && qba[i+1] != '+'){
            lines.push_back(QString::fromUtf8(tb));//push the line info into array
            tb.clear();//clears the buffer for new line
            newLineTrip = 0;
        }
    }
    lines.push_back(".ENDS");//Handeling the last line exception

    //Some tokens
    char* start = ".SUBCKT ";
    char* end  = ".ENDS";
    char* MMos = "M";
    char* XCall = "X";

    QRegExp rx(R"((\w+))");//word of any length
    QRegExp rx2(R"((\w+=\w+))");//word before = // for Mmos
    QRegExp rx3(R"(\w+=\w+\.\w+)");//word before and after includeing numbers with decimals.//fors Xcall
    QRegExp rx4(R"(/\s\w+)");//word after / for name of cellSbkt
    QRegExp rx5(R"([a-z0-9/<>_-]{1,20})");//encompasses a wide set with /, <> and _ undersore from 2 to 20 letters
    //\w+<\d>

    CellCBKT* tcell;

    for(uint i = 0; i < lines.size() ; i++)
    {
        lines[i].replace("\n","");
        lines[i].replace('+',"");
        uint count = 0;

        if(lines[i].contains(start))
        {
            QStringList name;
            int pos = 0;
            tcell = new CellCBKT();//creates a new cell evertime it find a .SBKT in the linesss

            while((pos = rx.indexIn(lines[i],pos)) != -1)//parse over each word in the line
            {
                if(count == 1)
                {
                    tcell->name = rx.cap(1);// store sbkt name
                }
                if(count >= 2)
                { // saves the port names
                    tcell->port.push_back(rx.cap(1));
                }
                pos += rx.matchedLength();//iterate pos over each word
                count++;
            }
        }
        ::MMos* tm;
        if(lines[i][0] == MMos)//Hits when a line has MMos
        {
            int pos = 0;
            count = 0;

            tm = new ::MMos();
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

        //Rus after all cells have been defined , Xcall will call only a defined cell
        if(lines[i][0] == XCall)//Hits when a line has Xcall
        {
            int pos = 0;
            ::XCall* xc = new ::XCall();
            bool hitStop = false;
            while((pos = rx5.indexIn(lines[i],pos)) != -1)//parse over each word in the line
            {
                if(rx5.cap(0) == '/')
                {
                    hitStop = true;
                }
                if(count == 0)//First word
                {
                    QString name = rx5.cap(0);
                    name.insert(0,'X');
                    xc->name = name;
                }
                if(!hitStop && count > 0)//first 4 items
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
                xc->cell = findCellFromName(name);
                pos += rx4.matchedLength();
            }

            tcell->xVec.push_back(xc);
        }
        if(lines[i].contains(end))
        {
            qInfo() << "---------";//lines[i];
            cells.push_back(tcell);
            tcell = NULL;
        }
    }
    return cells;
}

CellCBKT* NetlistParserBF::findCellFromName(QString name)
{
    for(uint i = 0 ; i< cells.size() ; i++){
        if(cells[i]->name == name){
            return cells[i];
        }
    }
}
