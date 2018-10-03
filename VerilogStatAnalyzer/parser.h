#pragma once

#include <vector>
#include <cmath>

#include "tokenizer.h"
#include "always.h"
#include "assign.h"
#include "case.h"
#include "declaration.h"
#include "enums.h"
#include "gate.h"
#include "constant.h"
#include "module.h"

const char* dumpFile = ".\\result.log";

// enable\disable checks
bool checkUnusedVars = false;
bool checkBitCapacityMismatch = false;
bool checkIncompleteCase = false;

std::vector<token> tokens;
std::vector<Node*> Operators; // operator nodes
std::vector<Variable*> Vars; // variable nodes

unsigned int pos = 0; // cursor

Variable* find_var(const char* name)
{
    for (unsigned int i = 0; i < Vars.size(); i++)
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
        var->Capacity = 1;
        Vars.push_back(var);
    }
    decl->Capacity = 1;
    
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
        decl->Capacity = abs(left_border - right_border) + 1;
        var->Capacity = decl->Capacity;
        pos += 2; // skip square brace
    }

    pos++; // skip semicolon at the end of declaration
}

// Important: still doesn't read operations on right hand side
void read_assign()
{
    if (tokens[pos].item == "assign")
    {
        pos++; // skip assign word
    }

    Assign* operation = new Assign();
    Constant* constValue = new Constant();
    operation->LHS = find_var(tokens[pos].item.c_str());
    pos += 2; // move to right hand side
    if (find_var(tokens[pos].item.c_str()) == NULL) // constant, not variable
    {
        constValue->Value = atoi(tokens[pos].item.c_str());
        Vars.push_back(constValue);
        operation->RHS = constValue;
    }
    else
    {
        operation->RHS = find_var(tokens[pos].item.c_str());
    }

    Operators.push_back(operation);

    pos += 2; // skip behind semicolon
}

void read_gate()
{
    GateType type;

    if (tokens[pos].item == "not")
        type = GATE_NOT;
    if (tokens[pos].item == "nand")
        type = GATE_NAND;
    if (tokens[pos].item == "and")
        type = GATE_AND;
    if (tokens[pos].item == "nor")
        type = GATE_NOR;
    if (tokens[pos].item == "or")
        type = GATE_OR;
    if (tokens[pos].item == "xor")
        type = GATE_XOR;
    if (tokens[pos].item == "xnor")
        type = GATE_XNOR;

    Gate* gate = new Gate();
    gate->Type = type;
    pos++;

    gate->Name = tokens[pos].item;
    pos += 2;

    gate->OutVar = find_var(tokens[pos].item.c_str());
    pos++;

    do
    {
        if (tokens[pos].item == ",")
        {
            pos++;
            continue;
        }

        gate->InVars.push_back(find_var(tokens[pos].item.c_str()));
        pos++;
    } while (tokens[pos].item != ")");

    Operators.push_back(gate);
}

// these are for future
void read_if() {}

void read_case()
{
    int start = tokens[pos].line;
    pos += 2; // skip "case" keyword and "(" symbol

    Case* block = new Case();
    block->StartLine = start;
    block->Switch = find_var(tokens[pos].item.c_str());
    pos += 2; // skip ")" symbol

    // TODO: read operations on the right of cases
    while (tokens[pos].item != "endcase")
    {
        if (tokens[pos].item == "default")
        {
            block->HasDefault = true;
            pos++;
            continue;
        }
        if (tokens[pos + 1].item == ",")
        {
            block->CaseCount++;
        }
        if (tokens[pos + 1].item == ":")
        {
            block->CaseCount++;
            pos += 2;
            read_assign();
            continue;
        }
        
        pos++;
    }
    
    pos++; // skip "endcase" keyword
    Operators.push_back(block);

    return;
}

void read_always()
{
    pos++; // skip "always" keyword
    pos += 2; // skip "@" and "(" - separated for readability
    bool complex = false;

    Always* block = new Always();

    // TODO: read events properly
    if (tokens[pos].item != "*")
    {
        while(tokens[pos].item != ")")
        {
            block->Events.push_back(find_var(tokens[pos].item.c_str()));
            pos++;
        }
        pos++; // skip ")"
    }
    else
    {
        pos += 2;
    }

    if (tokens[pos].item == "begin")
    {
        complex = true;
        pos++;
    }

    do
    {
        if (tokens[pos].item == "if")
        {
            read_if();
        }
        if (tokens[pos + 1].item == "=")
        {
            read_assign();
        }
        if (tokens[pos].item == "case")
        {
            read_case();
        }
    } while (complex && (tokens[pos].item != "end"));

    Operators.push_back(block);

    return;
}

void read_module()
{
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
            var_buffer->Capacity = 1;
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

        if (tokens[pos].item == "endmodule")
        {
            break;
        }

        pos++;
    }
}

/*
* Important: for now we assume than verilog syntax is correct
*/
bool readFile(const char* fileName)
{
    tokens = tokenize(fileName);

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

bool readConfig(const char* fileName)
{
    FILE *p_file = fopen(fileName, "rt");
    if (!p_file)
        return false;

    char check[128];
    int checked;
    while (!feof(p_file))
    {
        fscanf(p_file, "%s = %d", check, &checked);
        if (!strcmp(check, "UNUSED_VARS"))
        {
            checkUnusedVars = checked > 0;
        }
        if (!strcmp(check, "BIT_CAPACITY_MISMATCH"))
        {
            checkBitCapacityMismatch = checked > 0;
        }
        if (!strcmp(check, "INCOMPLETE_CASE_STATEMENT"))
        {
            checkIncompleteCase = checked > 0;
        }
    }

    fclose(p_file);
    return true;
}

// TODO: add always and case, refactor for readability
void analyzeUnusedVars(FILE* dump)
{
    for (unsigned int i = 0; i < Vars.size(); i++)
    {
        if (Vars[i]->nodeType == NODE_CONSTANT)
        {
            continue;
        }

        bool foundVar = false;
        Assign* assignBuffer;
        Gate* gateBuffer;

        for (unsigned int j = 0; j < Operators.size(); j++)
        {
            switch (Operators[j]->nodeType)
            {
                case NODE_ASSIGN:
                    assignBuffer = (Assign *)Operators[j];
                    if ((assignBuffer->LHS->Name == Vars[i]->Name)
                        || ((assignBuffer->RHS->nodeType == NODE_VARIABLE) && (((Variable *)assignBuffer->RHS)->Name == Vars[i]->Name)))
                    {
                        foundVar = true;
                        continue;
                    }
                    break;
                case NODE_GATE:
                    gateBuffer = (Gate *)Operators[j];
                    if (gateBuffer->OutVar->Name == Vars[i]->Name)
                    {
                        foundVar = true;
                        continue;
                    }
                    for (unsigned int k = 0; k < gateBuffer->InVars.size(); k++)
                    {
                        if (gateBuffer->InVars[k]->Name == Vars[i]->Name)
                        {
                            foundVar = true;
                            goto stopWalking;
                        }
                    }
                    break;
            }
        }
stopWalking:
        if (foundVar)
        {
            continue;
        }
        fprintf(dump, "VSA001: unused variable - %s\n", Vars[i]->Name.c_str());
    }
}

void analyzeBitCapacity(FILE* dump)
{
    Assign* assignBuffer;
    Variable* varBuffer;
    Constant* constBuffer;

    for (unsigned int i = 0; i < Operators.size(); i++)
    {
        if (Operators[i]->nodeType == NODE_ASSIGN)
        {
            assignBuffer = (Assign *)Operators[i];

            if (assignBuffer->RHS->nodeType == NODE_VARIABLE)
            {
                varBuffer = (Variable *)assignBuffer->RHS;
                if (assignBuffer->LHS->Capacity != varBuffer->Capacity)
                {
                    std::string leftName = assignBuffer->LHS->Name;
                    std::string rightName = varBuffer->Name;

                    fprintf(
                        dump,
                        "VSA002: bit capacity mismatch while assigning \"%s\" to \"%s\"\n",
                        rightName.c_str(),
                        leftName.c_str()
                    );
                }
            }
            if (assignBuffer->RHS->nodeType == NODE_CONSTANT)
            {
                constBuffer = (Constant *)assignBuffer->RHS;
                if (constBuffer->Value != pow(2.0, assignBuffer->LHS->Capacity))
                {
                    fprintf(
                        dump,
                        "VSA002: bit capacity mismatch while assigning %d to \"%s\"\n",
                        constBuffer->Value,
                        assignBuffer->LHS->Name.c_str()
                    );
                }
            }
        }
    }
}

void analyzeIncompleteCase(FILE* dump)
{
    Case* buffer;
    int maxCases = 0;
    for (unsigned int i = 0; i < Operators.size(); i++)
    {
        if (Operators[i]->nodeType == NODE_CASE)
        {
            buffer = (Case *)Operators[i];
            maxCases = pow(2.0, buffer->Switch->Capacity);
            if ((buffer->CaseCount < maxCases) && !buffer->HasDefault)
            {
                fprintf(dump, "VSA003: incomplete case statement at line %d", buffer->StartLine);
            }
        }
    }
}

void performAnalysis()
{
    FILE *p_file = fopen(dumpFile, "w");
    if (!p_file)
        return;

    if (checkUnusedVars)
    {
        analyzeUnusedVars(p_file);    
    }

    if (checkBitCapacityMismatch)
    {
        analyzeBitCapacity(p_file);
    }

    if (checkIncompleteCase)
    {
        analyzeIncompleteCase(p_file);
    }

    fclose(p_file);
}