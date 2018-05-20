#pragma once

#include "node.h"
#include "enums.h"

#include <string>

class Variable: public Node
{
public:
    Variable()
    {
        this->nodeType = NODE_VARIABLE;
    }
    std::string Name;
    int Type;
    int Capacity;
};
