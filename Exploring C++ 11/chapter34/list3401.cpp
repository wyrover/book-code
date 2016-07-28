// Listing 34-1. New BMI Program
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <string>
#include <vector>

/// Compute body-mass index from height in centimeters and weight in kilograms.
int compute_bmi(int height, int weight)
{
   return static_cast<int>(weight * 10000 / (height * height) + 0.5);
}

/// Skip the rest of the input line.
void skip_line(std::istream& in)
{
  in.ignore(std::numeric_limits<int>::max(), '\n');
}

/// Represent one person's record, storing the person's name, height, weight,
/// sex, and body-mass index (BMI), which is computed from the height and weight.
struct record
{
  record() : height_{0}, weight_{0}, bmi_{0}, sex_{'?'}, name_{}
  {}

  /// Get this record, overwriting the data members.
  /// Error-checking omitted for brevity.
  /// @return true for success or false for eof or input failure
  bool read(std::istream& in, int num)
  {
    std::cout << "Name " << num << ": ";
    std::string name{};
    if (not std::getline(in, name))
      return false;

    std::cout << "Height (cm): ";
    int height{};
    if (not (in >> height))
      return false;
    skip_line(in);

    std::cout << "Weight (kg): ";
    int weight{};
    if (not (in >> weight))
      return false;
    skip_line(in);

    std::cout << "Sex (M or F): ";
    char sex{};
    if (not (in >> sex))
      return false;
    skip_line(in);
    sex = std::toupper(sex, std::locale());

    // Store information into data members only after reading
    // everything successfully.
    name_ = name;
    height_ = height;
    weight_ = weight;
    sex_ = sex;
    bmi_ = compute_bmi(height_, weight_);
    return true;
  }

  /// Print this record to @p out.
  void print(std::ostream& out, int threshold)
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

  int height_;       ///< height in centimeters
  int weight_;       ///< weight in kilograms
  int bmi_;          ///< Body-mass index
  char sex_;         ///< 'M' for male or 'F' for female
  std::string name_; ///< Person’s name
};


/** Print a table.
 * Print a table of height, weight, sex, BMI, and name.
 * Print only records for which sex matches @p sex.
 * At the end of each table, print the mean and median BMI.
 */
void print_table(char sex, std::vector<record>& records, int threshold)
{
  std::cout << "Ht(cm) Wt(kg) Sex  BMI  Name\n";

  float bmi_sum{};
  long int bmi_count{};
  std::vector<int> tmpbmis{}; // store only the BMIs that are printed
                            // in order to compute the median
  for (auto rec : records)
  {
    if (rec.sex_ == sex)
    {
      bmi_sum = bmi_sum + rec.bmi_;
      ++bmi_count;
      tmpbmis.push_back(rec.bmi_);
      rec.print(std::cout, threshold);
    }
  }

  // If the vectors are not empty, print basic statistics.
  if (bmi_count != 0)
  {
    std::cout << "Mean BMI = "
              << std::setprecision(1) << std::fixed << bmi_sum / bmi_count
              << '\n';

    // Median BMI is trickier. The easy way is to sort the
    // vector and pick out the middle item or items.
    std::sort(tmpbmis.begin(), tmpbmis.end());
    std::cout << "Median BMI = ";
    // Index of median item.
    std::size_t i{tmpbmis.size() / 2};
    if (tmpbmis.size() % 2 == 0)
      std::cout << (tmpbmis.at(i) + tmpbmis.at(i-1)) / 2.0 << '\n';
    else
      std::cout << tmpbmis.at(i) << '\n';
  }
}

/** Main program to compute BMI. */
int main()
{
  std::locale::global(std::locale{""});
  std::cout.imbue(std::locale{});
  std::cin.imbue(std::locale{});

  std::vector<record> records{};
  int threshold{};

  std::cout << "Enter threshold BMI: ";
  if (not (std::cin >> threshold))
    return EXIT_FAILURE;
  skip_line(std::cin);

  std::cout << "Enter name, height (in cm),"
               " and weight (in kg) for each person:\n";
  record rec{};
  while (rec.read(std::cin, records.size()+1))
  {
    records.push_back(rec);
    std::cout << "BMI = " << rec.bmi_ << '\n';
  }

  // Print the data.
  std::cout << "\n\nMale data\n";
  print_table('M', records, threshold);
  std::cout << "\nFemale data\n";
  print_table('F', records, threshold);
}
