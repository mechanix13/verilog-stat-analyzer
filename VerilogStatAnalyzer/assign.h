#pragma once

#include "node.h"
#include "enums.h"

class Assign: public Node
{
public:
    Assign()
    {
        this->nodeType = NODE_ASSIGN;
    }

    Variable* LHS;
    Node* RHS;
};
