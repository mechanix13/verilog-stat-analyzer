#pragma once

#include "node.h"
#include <vector>

class Module: public Node
{
public:
    std::vector<Node*> Actions;
};
