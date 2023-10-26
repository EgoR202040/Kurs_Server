#include "Calculator.h"
#include <vector>
#include <limits>
#include <boost/numeric/conversion/cast.hpp>
double Calculator::calculate(std::vector<double>input_data)
{
    //std::cout << "Calc info:" << std::endl;

    double temp_res = 1;
    try {
        //std::cout <<elem <<std::endl;
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
