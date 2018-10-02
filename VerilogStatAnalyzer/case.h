#pragma once

#include "node.h"
#include "enums.h"

class Case: public Node
{
public:
    Case()
    {
        this->nodeType = NODE_CASE;
        this->CaseCount = 0;
        this->HasDefault = false;
        this->StartLine = 0;
    }

    Variable* Switch;
    int CaseCount;
    bool HasDefault;
    unsigned int StartLine;
};
