#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <boost/assign.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <numeric> // for std::accumulate
using namespace boost::assign;

typedef std::map<std::string, int> scoremap;

struct GetScore : std::unary_function <
    const scoremap::value_type&, int > {
    result_type operator()(argument_type entry) const
    {
        return entry.second;
    }
};

int main()
{
    scoremap subjectScores{{"Physics", 80}, {"Chemistry", 78},
        {"Statistics", 88}, {"Mathematics", 92}};
    boost::transform_iterator<GetScore,
          scoremap::iterator>
          first(subjectScores.begin(), GetScore()),
          last(subjectScores.end(), GetScore());
    std::cout << std::accumulate(first, last, 0) << '\n';
}

