// Listing 52-5. Coping with Conflicting Names
 1 #include <cmath>
 2 #include <numeric>
 3 #include <vector>
 4
 5 namespace stats {
 6   // Really bad name for a functor to compute sum of squares,
 7   // for use in determining standard deviation.
 8   class std
 9   {
10   public:
11     std(double mean) : mean_{mean} {}
12     double operator()(double acc, double x)
13     const
14     {
15       return acc + square(x - mean_);
16     }
17     double square(double x) const { return x * x; }
18   private:
19     double mean_;
20   };
21
22   // Really bad name for a function in the stats namespace.
23   // It computes standard deviation.
24   double stats(::std::vector<double> const& data)
25   {
26     double std{0.0}; // Really, really bad name for a local variable
27     if (not data.empty())
28     {
29       double sum{::std::accumulate(data.begin(), data.end(), 0.0)};
30       double mean{sum / data.size()};
31       double sumsq{::std::accumulate(data.begin(), data.end(), 0.0,
32                    stats::std(mean))};
33       double variance{sumsq / data.size() - mean * mean};
34       std = ::std::sqrt(variance);
35     }
36     return std;
37   }
38 }
