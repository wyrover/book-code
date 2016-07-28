// Listing 41-8. The vital_stats Class to Record a Person’s Vital Statistics
#include <istream>
#include <ostream>
#include <string>

class vital_stats
{
public:
  vital_stats() : height_{0}, weight_{0}, bmi_{0}, sex_{'?'}, name_{}
  {}

  bool read(std::istream& in, int num);
  void print(std::ostream& out, int threshold) const;

private:
  int compute_bmi() const; ///< Return BMI, based on height_ and weight_
  int height_;             ///< height in centimeters
  int weight_;             ///< weight in kilograms
  int bmi_;                ///< Body-mass index
  char sex_;               ///< 'M' for male or 'F' for female
  std::string name_;       ///< Person’s name
};
