// Ex8_06.cpp
// Checking out lognormal distributions
#include <random>                                          // For distributions and random number generators
#include <algorithm>                                       // For generate(), for_each(), max_element(), transform()
#include <numeric>                                         // For accumulate() 
#include <iterator>                                        // For back_inserter() 
#include <vector>                                          // For vector container
#include <map>                                             // For map container
#include <cmath>                                           // For pow(), round(), log() functions
#include <iostream>                                        // For standard streams
#include <iomanip>                                         // For stream manipulators
#include <string>
using std::string;
using Params = std::lognormal_distribution<>::param_type;

// Template to plot a distribution from a range of sample values
template<typename Iter>
void dist_plot(Iter& beg_iter, Iter& end_iter, size_t width = 90)
{
    // Create data for distribution plot
    std::map<int, size_t> plot_data;                         // Elements are pair<value, frequency>
    // Make sure all values are present in the plot
    auto pr = std::minmax_element(beg_iter, end_iter, [](const double v1, const double v2) {
        return v1 < v2;
    });

    for (int n {static_cast<int>(*pr.first)}; n < static_cast<int>(*pr.second); ++n)
        plot_data.emplace(n, 0);

    // Create the plot data
    std::for_each(beg_iter, end_iter,
    [&plot_data](double value) {
        ++plot_data[static_cast<int>(std::round(value))];
    });
    // Find maximum frequency to be plotted - must fit within page width
    size_t max_f {std::max_element(std::begin(plot_data), std::end(plot_data),
                                   [](const std::pair<int, int>& v1, const std::pair<int, int>& v2)
    {
        return v1.second < v2.second;
    })->second};
    // Draw distribution as histogram
    std::for_each(std::begin(plot_data), std::end(plot_data),
    [max_f, width](const std::pair<int, int>& v) {
        if ((width * v.second) / max_f > 0)
            std::cout << std::setw(3) << v.first << " -| " << string((width * v.second) / max_f, '*') << std::endl;
    });
}

int main()
{
    std::random_device rd;
    std::default_random_engine rng {rd()};
    std::lognormal_distribution<> log_norm;
    double mu {}, sigma {};
    const size_t sample_count {20000};
    std::vector<double> values(sample_count);
    std::vector<double> log_values;

    while (true) {
        std::cout << "\nEnter values for the mean and standard deviation, or Ctrl+Z to end: ";

        if ((std::cin >> mu).eof()) break;

        std::cin >> sigma;
        log_norm.param(Params {mu, sigma});
        std::generate(std::begin(values), std::end(values), [&log_norm, &rng] { return log_norm(rng); });
        // Create data to plot lognormal curve
        dist_plot(std::begin(values), std::end(values));
        // Create logarithms of values
        std::vector<double> log_values;
        std::transform(std::begin(values), std::end(values), std::back_inserter(log_values),
        [](double v) {
            return log(v);
        });
        // Create data to plot curve for logarithms of values
        std::cout << "\nThe distribution for logarithms of the values:\n";
        dist_plot(std::begin(log_values), std::end(log_values));
        // Get the mean and standard deviation -  for the logarithms of the values
        double mean {std::accumulate(std::begin(log_values), std::end(log_values), 0.0) / log_values.size()};
        std::transform(std::begin(log_values), std::end(log_values), std::begin(log_values),
        [&mean](double value) {
            return std::pow(value - mean, 2);
        });
        double s_dev {std::sqrt(std::accumulate(std::begin(log_values), std::end(log_values), 0.0) / (log_values.size() - 1))};
        std::cout << "For generated values, mean = " << mean << " standard deviation = " << s_dev << std::endl;
    }
}