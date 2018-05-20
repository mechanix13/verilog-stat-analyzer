#pragma once

#include "node.h"
#include "variable.h"
#include "enums.h"
#include <vector>

class Always: public Node
{
public:
    Always()
    {
        this->nodeType = NODE_ALWAYS;
    }

    std::vector<Variable*> Events;
    std::vector<Node*> Operations;
};
