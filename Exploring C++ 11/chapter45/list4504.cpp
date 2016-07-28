// Listing 45-4. Visualizing an Exception
#include <exception>
#include <iostream>
#include <string>

/// Make visual the construction and destruction of objects.
class visual
{
public:
  visual(std::string const& what)
  : id_{serial_}, what_{what}
  {
    ++serial_;
    print("");
  }
  visual(visual const& ex)
  : id_{ex.id_}, what_{ex.what_}
  {
    print("copy ");
  }
  ~visual()
  {
    print("~");
  }
  void print(std::string const& label)
  const
  {
    std::cout << label << "visual(" << what_ << ": " << id_ << ")\n";
  }
private:
  static int serial_;
  int const id_;
  std::string const what_;
};

int visual::serial_{0};

void count_down(int n)
{
  std::cout << "start count_down(" << n << ")\n";
  visual v{"count_down local"};
  try
  {
    if (n == 3)
      throw visual("exception");
    else if (n > 0)
      count_down(n - 1);
  }
  catch (visual ex)
  {
    ex.print("catch ");
    throw;
  }
  std::cout << "end count_down(" << n << ")\n";
}

int main()
{
  try
  {
    count_down(2);
    count_down(4);
  }
  catch (visual const ex)
  {
    ex.print("catch ");
  }
  std::cout << "All done!\n";
}
