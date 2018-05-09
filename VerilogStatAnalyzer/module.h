#pragma once

#include "node.h"
#include <vector>

class Module: public Node
{
public:
    std::string Name;
    std::vector<Variable*> Ports;
    std::vector<Node*> Actions;
};
