// Ex7_05.cpp
// Apply some algorithms to plotting curves
// To get the output with the correct aspect ratio, set the characters
// in the standard output stream destination to a square font, such as 8 x 8 pixels
#include <iostream>                                      // For standard streams
#include <iterator>                                      // For iterators and begin() and end()
#include <string>                                        // For string class
#include <vector>                                        // For vector container
#include <algorithm>                                     // For algorithms
#include <cmath>                                         // For sin(), cos()
using std::string;
using Point = std::pair<double, double>;

static const double pi {3.1415926};

// Function template to plot a range of x,y values on cout
template<typename Iterator>
void plot(Iterator begin_iter, Iterator end_iter, string name = "Curve", size_t n_x = 100)
{
    // n_x is plot width in characters, so it's the number of characters along the x axis
    // Comparison functions for x and for y
    auto x_comp = [](const Point & p1, const Point & p2) {
        return p1.first < p2.first;
    };
    auto y_comp = [](const Point & p1, const Point & p2) {
        return p1.second < p2.second;
    };
    // Minimum and maximum x values
    auto min_x = std::min_element(begin_iter, end_iter, x_comp)->first;
    auto max_x = std::max_element(begin_iter, end_iter, x_comp)->first;
    // Step length for output - same step applies to x and y
    double step {(max_x - min_x) / (n_x + 1)};
    // Minimum and maximum y values
    auto min_y = std::min_element(begin_iter, end_iter, y_comp)->second;
    auto max_y = std::max_element(begin_iter, end_iter, y_comp)->second;
    size_t nrows {1 + static_cast<size_t>(1 + (max_y - min_y) / step)};
    std::vector<string> rows(nrows, string(n_x + 1, ' '));

    // Create x-axis at y=0 if this is within range of points
    if (max_y > 0.0 && min_y <= 0.0)
        rows[1 + static_cast<size_t>(max_y / step)] = string(n_x + 1, '-');

    // Create y-axis at x=0 if this is within range of points
    if (max_x > 0.0 && min_x <= 0.0) {
        size_t x_axis {static_cast<size_t>(-min_x / step)};
        std::for_each(std::begin(rows), std::end(rows),
        [x_axis](string & row) {
            row[x_axis] = row[x_axis] == '-' ? '+' : '|';
        });
    }

    std::cout << "\n\n     " << name << ":\n\n";
    // Generate the rows for output
    auto y {max_y};                             // Upper y for current output row

    for (auto& row : rows) {
        // Find points to be included in an output row
        std::vector<Point> row_pts;               // Stores points for this row
        std::copy_if(begin_iter, end_iter, std::back_inserter(row_pts),
        [&y, &step](const Point & p) {
            return p.second < y + step && p.second >= y;
        });
        std::for_each(std::begin(row_pts), std::end(row_pts),  // Set * for pts in the row
        [&row, min_x, step](const Point & p) {
            row[static_cast<size_t>((p.first - min_x) / step)] = '*';
        });
        y -= step;
    }

    // Output the plot - which is all the rows.
    std::copy(std::begin(rows), std::end(rows), std::ostream_iterator<string> {std::cout, "\n"});
    std::cout << std::endl;
}

// Generate x,y points on curve y = sin(x) for x values 0 to 4?
std::vector<Point> sine_curve(size_t n_pts = 100)
{
    // n_pts is number of data points for the curve
    std::vector<double> x_values(n_pts);
    double value {};
    double step {4 * pi / (n_pts - 1)};
    std::generate(std::begin(x_values), std::end(x_values),
    [&value, &step]() {
        double v {value};
        value += step;
        return v;
    });
    std::vector<Point> curve_pts;
    std::transform(std::begin(x_values), std::end(x_values), std::back_inserter(curve_pts),
    [](double x) {
        return Point {x, sin(x)};
    });
    return curve_pts;
}

// Generate x,y points on a cardioid defined by parametric equations:
// x = a(2cos(t) - cos(2t))  y = a(sin(t) - sin(2t)
// A cardioid is the path of a point on one circle
// that is rolling around another with the same radius
std::vector<Point> cardioid_curve(double r = 1.0, size_t n_pts = 100)
{
    // n_pts is number of data points
    double step = 2 * pi / (n_pts - 1);                 // Step length for x and y
    double t_value {};                                  // Curve parameter
    // Create parameter values that define the curve
    std::vector<double> t_values(n_pts);
    std::generate(std::begin(t_values), std::end(t_values), [&t_value, step]() {
        auto value = t_value;
        t_value += step;
        return value;
    });
    // Function to define an x,y point on the cardioid for a given t
    auto cardioid = [r](double t) {
        return Point {r*(2 * cos(t) + cos(2 * t)), r*(2 * sin(t) + sin(2 * t))};
    };
    // Create the points for the cardioid
    std::vector<Point> curve_pts;
    std::transform(std::begin(t_values), std::end(t_values), std::back_inserter(curve_pts),
                   cardioid);
    return curve_pts;
}

int main()
{
    auto curve1 = sine_curve(50);
    plot(std::begin(curve1), std::end(curve1), "Sine curve", 90);
    auto curve2 = cardioid_curve(1.5, 120);
    plot(std::begin(curve2), std::end(curve2), "Cardioid", 60);
}