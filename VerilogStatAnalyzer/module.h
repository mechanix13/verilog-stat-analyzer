#pragma once

#include "node.h"
#include "enums.h"
#include <vector>

class Module: public Node
{
public:
    Module() 
    {
        this->nodeType = NODE_MODULE;
    }

    std::string Name;
    std::vector<Variable*> Ports;
    std::vector<Node*> Actions;
};
