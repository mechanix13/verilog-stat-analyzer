#pragma once

#include "node.h"
#include "enums.h"

class Gate: public Node
{
public:
    Gate()
    {
        this->nodeType = NODE_GATE;
    }

    GateType Type;
    std::string Name;
    Variable* OutVar;
    std::vector<Variable*> InVars;
};
