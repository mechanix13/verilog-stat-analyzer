#pragma once

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

enum VarType {
    VAR_INPUT,
    VAR_OUTPUT,
    VAR_REG,
    VAR_WIRE
};
