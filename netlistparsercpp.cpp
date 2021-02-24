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

    char* start = ".SUBCKT ";
    char* end  = ".ENDS";
    char* MMos = "M";

    //Regexp saves
    //(?<=\bSUBCKT\s)(\w+) to search word after SUBCKT
    //(\w+) word of any length

    QRegExp rx(R"((\w+))");//word of any length
    QRegExp rx2(R"((\w+=\w+))");//word before = // for Mmos
    QRegExp rx3(R"(\w+=\w+\.\w+)");//word before and after includeing numbers with decimals.//fors Xcall
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
//            cells.push_back(tcell);//populate the array of cells
        }
        if(lines[i][0] == MMos)
        {
            int pos = 0;
            count = 0;

            ::MMos* tm = new ::MMos();
            while((pos = rx.indexIn(lines[i],pos)) != -1)//parse over each word in the line
            {
                if(count == 0)
                {
                    tm->name = rx.cap(1);
                }
                if(count >= 1  && count <= 4)
                {
                   tm->pins.push_back(rx.cap(1));
                }
                pos += rx.matchedLength();//iterate pos over each word
                count++;
            }
            pos = 0;
            while((pos = rx2.indexIn(lines[i],pos)) != -1)
            {
                //for device properties
                QString ts = rx2.cap(1);//.remove(QRegularExpression("\\="));
                ts.truncate(ts.lastIndexOf(QChar('=')));
                Device d;
                d.paramName = ts;
                d.paramValue = rx2.cap(1).remove(QRegularExpression("\\w+="));

                pos += rx2.matchedLength();//iterate pos over each word
            }
            tcell->mVec.push_back(tm);
        }
        if(lines[i][0] == 'X')
        {

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
