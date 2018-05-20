#include "parser.h"

#include <vector>
#include <string>
#include <iostream>

const char* file = "D:\\test\\buff.v";

int main()
{
    readFile(file);
    readConfig(".\\analyze.conf");
    performAnalysis();

    return 0;
}
