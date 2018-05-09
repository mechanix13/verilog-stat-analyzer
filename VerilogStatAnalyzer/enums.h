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
    VAR_INPUT = 1,
    VAR_OUTPUT = 2,
    VAR_REG = 4,
    VAR_WIRE = 8
};
