// Listing 41-10. The vital_stats.cpp Source File
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <string>

#include "vital_stats.hpp"

/// Skip the rest of the input line.
/// @param in the input stream
void skip_line(std::istream& in)
{
  in.ignore(std::numeric_limits<int>::max(), '\n');
}

int vital_stats::compute_bmi()
const
{
   return static_cast<int>(weight_ * 10000 / (height_ * height_) + 0.5);
}

bool vital_stats::read(std::istream& in, int num)
{
  std::cout << "Name " << num << ": ";
  if (not std::getline(in, name_))
    return false;

  std::cout << "Height (cm): ";
  if (not (in >> height_))
    return false;
  skip_line(in);

  std::cout << "Weight (kg): ";
  if (not (in >> weight_))
    return false;
  skip_line(in);

  std::cout << "Sex (M or F): ";
  if (not (in >> sex_))
    return false;
  skip_line(in);
  sex_ = std::toupper(sex_, std::locale{});

  bmi_ = compute_bmi();
 
 return true;
}

void vital_stats::print(std::ostream& out, int threshold)
const
{
  out << std::setw(6) << height_
      << std::setw(7) << weight_
      << std::setw(3) << sex_
      << std::setw(6) << bmi_;
  if (bmi_ >= threshold)
    out << '*';
  else
    out << ' ';
  out << ' ' << name_ << '\n';
}
