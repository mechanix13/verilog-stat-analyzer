#pragma once

#include <vector>

#include "tokenizer.h"
#include "always.h"
#include "assign.h"
#include "declaration.h"
#include "enums.h"
#include "gate.h"
#include "module.h"

std::vector<token> tokens;
std::vector<Node*> Operators;
std::vector<Variable*> Vars;

unsigned int pos = 0;

Variable* find_var(const char* name)
{
    for (int i = 0; i < Vars.size(); i++)
    {
        if (Vars[i]->Name == name)
            return Vars[i];
    }

    return NULL;
}

void read_declaration() {
    VarType type;

    if (tokens[pos].item == "input")
        type = VAR_INPUT;
    if (tokens[pos].item == "output")
        type = VAR_OUTPUT;
    if (tokens[pos].item == "reg")
        type = VAR_REG;
    if (tokens[pos].item == "wire")
        type = VAR_WIRE;

    pos++;
    Variable* var = find_var(tokens[pos].item.c_str());
    if (var == NULL)
    {
        var = new Variable();
        var->Name = tokens[pos].item;
        var->Type = 0;
        Vars.push_back(var);
    }

    var->Type |= type;
    pos++; // skip semicolon at the end of declaration
}

void read_assign() {}
void read_gate() {}
void read_always() {}
void read_if() {}

void read_module() {
    Module* module_buffer;
    Variable* var_buffer;
    
    module_buffer = new Module();
    module_buffer->Name = tokens[pos].item;
    Operators.push_back(module_buffer);

    pos++;
    if (tokens[pos].item == "(")
    {
        pos++;
        while (tokens[pos].item != ")")
        {
            if (tokens[pos].item  == ",")
            {
                pos++;
                continue;
            }

            // todo: add capacity
            var_buffer = new Variable();
            var_buffer->Name = tokens[pos].item;
            Vars.push_back(var_buffer);
            module_buffer->Ports.push_back(var_buffer);

            pos++;
        }
    }

    while (tokens[pos].item != "endmodule")
    {
        if ((tokens[pos].item == "input")
            || (tokens[pos].item == "output")
            || (tokens[pos].item == "reg")
            || (tokens[pos].item == "wire"))
        {
            read_declaration();
            continue;
        }

        if (tokens[pos].item == "always")
        {
            read_always();
        }

        if (tokens[pos].item == "assign")
        {
            read_assign();
        }

        if ((tokens[pos].item == "not")
            || (tokens[pos].item == "nand")
            || (tokens[pos].item == "and")
            || (tokens[pos].item == "nor")
            || (tokens[pos].item == "or")
            || (tokens[pos].item == "xor")
            || (tokens[pos].item == "xnor"))
        {
            read_gate();
        }

        pos++;
    }
}

/*
* Important: for now we assume than verilog syntax is correct
*/
bool read_file(const char* file_name)
{
    tokens = tokenize(file_name);

    for (pos = 0; pos < tokens.size(); pos++)
    {
        if (tokens[pos].item == "module")
        {
            pos++;
            read_module();
        }
    }

    return true;
}
