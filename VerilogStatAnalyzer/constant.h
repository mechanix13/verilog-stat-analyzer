#pragma once

#include "variable.h"
#include "enums.h"

class Constant: public Variable
{
public:
    Constant()
    {
        this->nodeType = NODE_CONSTANT;
    }
    int Value;
};