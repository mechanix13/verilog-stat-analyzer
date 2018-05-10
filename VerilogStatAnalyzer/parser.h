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
    int type;
    
    Declaration* decl = new Declaration;
    if (tokens[pos].item == "input")
        type = VAR_INPUT;
    if (tokens[pos].item == "output")
        type = VAR_OUTPUT;
    if (tokens[pos].item == "reg")
        type = VAR_REG;
    if (tokens[pos].item == "wire")
        type = VAR_WIRE;

    decl->Type = type;
    pos++;

    decl->VarName = tokens[pos].item;
    Operators.push_back(decl);

    Variable* var = find_var(tokens[pos].item.c_str());
    if (var == NULL)
    {
        var = new Variable();
        var->Name = tokens[pos].item;
        var->Type = 0;
        Vars.push_back(var);
    }
    
    if (var->Type < 0)
        var->Type = 0;

    var->Type |= type;
    pos++;
    if (tokens[pos].item == "[")
    {
        pos++;
        int left_border = atoi(tokens[pos].item.c_str());
        pos += 3;
        int right_border = atoi(tokens[pos].item.c_str());
        decl->Capacity = left_border > right_border ? left_border : right_border;
        var->Capacity = decl->Capacity
        pos += 2; // skip square brace
    }

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
