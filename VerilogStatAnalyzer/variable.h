#pragma once

#include "node.h"
#include "enums.h"

#include <string>

class Variable: public Node
{
public:
    std::string Name;
    std::string Type;
    int capacity;
};