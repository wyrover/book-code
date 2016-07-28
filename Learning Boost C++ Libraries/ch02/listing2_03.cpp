#include <boost/tuple/tuple.hpp>
#include <vector>

boost::tuple<size_t, size_t, double>
getBestTransactDays(std::vector<double> prices)
{
    double min = std::numeric_limits<double>::max();
    double gain = 0.0, max_gain = 0.0;
    size_t min_day, max_day;
    size_t buy_day;

    for (size_t i = 0, days = prices.size(); i < days; ++i) {
        if (prices[i] < min) {
            min = prices[i];
            min_day = i;
        } else if ((gain = prices[i] - min) > max_gain) {
            max_gain = gain;
            buy_day = min_day;
            max_day = i;
        }
    }

    return boost::make_tuple(buy_day, max_day, max_gain);
}

int main()
{
}
