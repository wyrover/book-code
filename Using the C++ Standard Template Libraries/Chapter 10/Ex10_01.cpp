// Ex10_01.cpp
// Least squares regression
#include <numeric>                                       // For accumulate(), inner_product()
#include <vector>                                        // For vector container
#include <iostream>                                      // For standard streams
#include <iomanip>                                       // For stream manipulators
#include <fstream>                                       // For file streams
#include <iterator>                                      // For iterators and begin() and end()
#include <string>                                        // For string class
using std::string;

int main()
{
    // File contains country_name renewables_per_person kwh_cost
    string file_in {"G:/Beginning_STL/renewables_vs_kwh_cost.txt"};
    std::ifstream in {file_in};

    if (!in) {                                    // Verify  we have a file
        std::cerr << file_in << " not open." << std::endl;
        exit(1);
    }

    std::vector<double> x;                        // Renewables per head
    std::vector<double> y;                        // Corresponding cost for a kilowatt hour
    // Read the file and show the data
    std::cout << "   Country   " << " Watts per Head " << " kwh cost(cents) " << std::endl;

    while (true) {
        string country;
        double renewables {};
        double kwh_cost {};

        if ((in >> country).eof()) break;                          // EOF read - we are done

        in >> renewables >> kwh_cost;
        x.push_back(renewables);
        y.push_back(kwh_cost);
        std::cout << std::left << std::setw(12) << country         // Output the record
                  << std::right
                  << std::fixed << std::setprecision(2) << std::setw(12) << renewables
                  << std::setw(16) << kwh_cost << std::endl;
    }

    auto n = x.size();                                            // Number of points
    auto sx = std::accumulate(std::begin(x), std::end(x), 0.0);   // Sum of x values
    auto sy = std::accumulate(std::begin(y), std::end(y), 0.0);   // Sum of y values
    auto mean_x = sx / n;                                         // Mean of x values
    auto mean_y = sy / n;                                         // Mean of y values
    // Sum of x*y values and sum of x-squared
    auto sxy = std::inner_product(std::begin(x), std::end(x), std::begin(y), 0.0);
    auto sx_2 = std::inner_product(std::begin(x), std::end(x), std::begin(x), 0.0);
    double a {}, b {};                                            // Line coefficients
    auto num = n * sxy - sx * sy;                                 // Numerator for a
    auto denom = n * sx_2 - sx * sx;                              // Denominator for a
    a = num / denom;
    b = mean_y - a * mean_x;
    std::cout << std:: fixed << std::setprecision(3) << "\ny = "  // Output equation
              << a << "*x + " << b << std::endl;                  // for regression line
}