#pragma once

#include "node.h"
#include "enums.h"

class Declaration: public Node
{
public:
    VarType Type;
    std::string Name;
};
