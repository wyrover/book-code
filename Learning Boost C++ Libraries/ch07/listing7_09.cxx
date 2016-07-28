 1 #include <boost/bind.hpp>
 2 
 3 std::vector<double> nums, raisedToThree;
 4 std::transform(nums.begin(), nums.end(),
 5                std::back_inserter(raisedToThree),
 6                boost::bind(pow, _1, 3));

