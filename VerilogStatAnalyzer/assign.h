#pragma once

#include "node.h"

class Assign: public Node
{
public:
    Variable* LHS;
    Node* RHS;
};
