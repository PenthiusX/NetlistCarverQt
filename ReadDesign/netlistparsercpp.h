#pragma once

#include "NetlistParser_I.h"
#include "cellcbkt.h" //base Containers file

class NetlistParserBF : public NetlistParser_I
{
    public:
        NetlistParserBF();
        virtual ~NetlistParserBF();
        std::vector<CellCBKT*> parse(QString path);
        std::vector<CellCBKT*> parse(QString path,char hint);

    private:
        CellCBKT* findCellFromName(QString name);
        QString readStringFromQrc(QString Filename);
        std::map<QString,QString> getPortNameMatchMap(std::vector<QString> xc ,std::vector<QString> mcr);
        std::vector<Device> getPortNameMatchArray(std::vector<QString> xc ,std::vector<QString> mcr);

        std::vector<QString> lines;
        std::vector<CellCBKT*> cells;
};
