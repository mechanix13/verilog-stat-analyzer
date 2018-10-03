#pragma once

enum NodeType
{
    NODE_MODULE,
    NODE_GATE,
    NODE_ASSIGN,
    NODE_ALWAYS,
    NODE_IF,
    NODE_CASE,
    NODE_DECLARATION,
    NODE_VARIABLE,
    NODE_CONSTANT,
};

enum GateType 
{
    GATE_NOT,
    GATE_NAND,
    GATE_AND,
    GATE_NOR,
    GATE_OR,
    GATE_XOR,
    GATE_XNOR
};

enum AlwaysEvent
{
    EVENT_POSEDGE,
    EVENT_NEGEDGE
};

// variable type definitions
const unsigned int VAR_INPUT = 1;
const unsigned int VAR_OUTPUT = 2;
const unsigned int VAR_REG = 4;
const unsigned int VAR_WIRE = 8;
