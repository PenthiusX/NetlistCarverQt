#pragma once

#include "I_Parser.h"
#include "cellcbkt.h"


class NetlistParserCpp /*: public I_Parser*/
{
private:
    std::vector<QString> lines;

    std::vector<CellCBKT*> cells;
    CellCBKT* tcell;

public:
    NetlistParserCpp(){}
    ~NetlistParserCpp()
    {
        for(auto ce : cells){delete ce;}
        lines.clear();
    }

    QString ReadStringFromQrc(QString Filename)
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

    std::vector<CellCBKT*> parseFile(QString path)
    {
        qInfo() << "path" << path;
        QByteArray qba = ReadStringFromQrc(path).toLocal8Bit();
        //        const char* p = qba;
        QByteArray tb;//temp buffer
        for(uint i = 0 ; i < qba.length(); i++)
        {
            tb.push_back(qba[i]);
            if(qba[i] == '\n')
            {
                lines.push_back(QString::fromUtf8(tb));//push the line info into array
                tb.clear();//clears the buffer for new line
            }
        }

        char* start = ".SUBCKT ";
        char* end  = ".ENDS";
        char* mos = "M";

        //Regexp saves
        //(?<=\bSUBCKT\s)(\w+) to search word after SUBCKT
        //(\w+) word of any length

        for(uint i = 0; i < lines.size() ; i++)
        {
            lines[i].replace("\n","");
            if(lines[i].contains(start))
            {
                QRegExp rx(R"((\w+))");//word of any length
                QString str = lines[i];
                QStringList name;
                int pos = 0;
                uint count = 0;

                tcell = new CellCBKT();//creates a new cell evertime it find a .SBKT in the line

                while((pos = rx.indexIn(str,pos)) != -1)//parse over each word in the line
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
                cells.push_back(tcell);//populate the array of cells
            }
            if(lines[i][0] == mos){
                qInfo() << lines[i];
            }
            if(lines[i][0] == 'X'){
                qInfo() << lines[i];
            }
            if(lines[i].contains(end)){
                qInfo() << "---------";//lines[i];
            }
        }
        return cells;
    }
};
