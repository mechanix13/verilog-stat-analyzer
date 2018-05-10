#pragma once

#include "node.h"
#include "enums.h"

class Declaration: public Node
{
public:
    int Type;
    int Capacity;
    std::string VarName;
};
