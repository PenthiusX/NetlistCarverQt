#pragma once

#include "I_Parser.h"
#include "cellcbkt.h" //base Containers file

class NetlistParserBF : public I_Parser
{
    public:
        NetlistParserBF();
        virtual ~NetlistParserBF();
        std::vector<CellCBKT*> parse(QString path);

    private:
        CellCBKT* findCellFromName(QString name);
        QString ReadStringFromQrc(QString Filename);
        std::vector<QString> lines;
        std::vector<CellCBKT*> cells;
};
