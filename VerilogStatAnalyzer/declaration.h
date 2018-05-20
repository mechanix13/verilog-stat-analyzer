#pragma once

#include "node.h"
#include "enums.h"

class Declaration: public Node
{
public:
    Declaration()
    {
        this->nodeType = NODE_DECLARATION;
    }

    int Type;
    int Capacity;
    std::string VarName;
};
