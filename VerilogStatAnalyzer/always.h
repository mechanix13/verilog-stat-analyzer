#pragma once

#include "node.h"
#include "variable.h"
#include <vector>

class Always: public Node
{
public:
    std::vector<Variable*> Events;
    std::vector<Node*> Operations;
};
