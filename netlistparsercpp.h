#pragma once

#include "I_Parser.h"
#include "cellcbkt.h" //base Containers file

class NetlistParserCpp : public I_Parser
{
    public:
        NetlistParserCpp();
        virtual ~NetlistParserCpp();

        QString ReadStringFromQrc(QString Filename);
        std::vector<CellCBKT*> parse(QString netlistFilePath);

    private:
        std::vector<QString> lines;
        std::vector<CellCBKT*> cells;
        CellCBKT* tcell;
};
