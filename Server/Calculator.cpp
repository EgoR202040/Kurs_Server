#include "Calculator.h"
#include <vector>
#include <limits>
#include <boost/numeric/conversion/cast.hpp>
Calculator::Calculator(std::vector<double>input_data)
{
    double temp_res = 1;
        for(auto elem:input_data) {
            temp_res = boost::numeric_cast<double>(temp_res*=elem);
        }
        results = temp_res;
};
double Calculator::send_res(){return results;}
