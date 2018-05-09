#pragma once

#include "tokenizer.h"
#include "always.h"
#include "assign.h"
#include "declaration.h"
#include "enums.h"
#include "gate.h"
#include "module.h"

bool read_file(const char* file_name)
{
    std::vector<token> tokens = tokenize(file_name);

    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i].item << " ";
    }

    std::cout << std::endl;

    return true;
}
