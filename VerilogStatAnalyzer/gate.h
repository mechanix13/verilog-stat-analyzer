#pragma once

#include "node.h"
#include "enums.h"

class Gate: public Node
{
public:
    GateType Type;
    std::string Name;
    Variable* OutVar;
    std::vector<Variable*> InVars;
};
