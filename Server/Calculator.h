#pragma once
#include <vector>
#include <iostream>
class Calculator
{
    std::vector<int> vector_result;
    std::vector<uint32_t> input_data;
public:
    double calculate(std::vector<double>input_data);
};