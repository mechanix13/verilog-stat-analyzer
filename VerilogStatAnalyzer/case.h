#pragma once

#include "node.h"
#include "enums.h"

class Case: public Node
{
public:
    Case()
    {
        this->nodeType = NODE_CASE;
    }

    Variable* Switch;
    Node* RHS;
};
