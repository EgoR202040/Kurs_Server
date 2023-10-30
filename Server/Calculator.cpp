#include "Calculator.h"
#include <vector>
#include <limits>
#include <boost/numeric/conversion/cast.hpp>
double Calculator::calculate(std::vector<double>input_data)
{
    double temp_res = 1;
    try {
        for(auto elem:input_data) {
            temp_res = boost::numeric_cast<double>(temp_res*=elem);
        }
        return temp_res;
    } catch (const boost::numeric::negative_overflow &e) {
        return std::numeric_limits<double>::lowest();
    } catch (const boost::numeric::positive_overflow &e) {
        return std::numeric_limits<double>::max();
    }
};
