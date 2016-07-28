// Listing 41-9. The vital_stats.hpp Header File
#ifndef VITAL_STATS_HPP_
#define VITAL_STATS_HPP_

#include <iosfwd>
#include <string>

class vital_stats
{
public:
  /// Constructor. Initialize everything to zero or other “empty” value.
  vital_stats() : height_{0}, weight_{0}, bmi_{0}, sex_{'?'}, name_{}
  {}

  /// Get this record, overwriting the data members.
  /// Error-checking omitted for brevity.
  /// @param in the input stream
  /// @param num a serial number, for use when prompting the user
  /// @return true for success or false for eof or input failure
  bool read(std::istream& in, int num);

  /// Print this record to @p out.
  /// @param out the output stream
  /// @param threshold mark records that have a BMI >= this threshold
  void print(std::ostream& out, int threshold) const;

  /// Return the BMI.
  int bmi() const { return bmi_; }
  /// Return the height in centimeters.
  int height() const { return height_; }
  /// Return the weight in kilograms.
  int weight() const { return weight_; }
  /// Return the sex: 'M' for male or 'F' for female
  char sex() const { return sex_; }
  /// Return the person’s name.
  std::string const& name() const { return name_; }

private:
  /// Return BMI, based on height_ and weight_
  /// This is called only from read().
  int compute_bmi() const;
  int height_;             ///< height in centimeters
  int weight_;             ///< weight in kilograms
  int bmi_;                ///< Body-mass index
  char sex_;               ///< 'M' for male or 'F' for female
  std::string name_;       ///< Person’s name
};

#endif
