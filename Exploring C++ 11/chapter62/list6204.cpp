// Listing 62-4. Copying a File Named on the Command Line
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "usage: " << argv[0] << " INPUT OUTPUT\n";
    return EXIT_FAILURE;
  }
  std::ifstream input{argv[1]};
  if (not input)
  {
    std::perror(argv[1]);
    return EXIT_FAILURE;
  }
  std::ofstream output{argv[2]};
  if (not output)
  {
    std::perror(argv[2]);
    return EXIT_FAILURE;
  }

  input.exceptions(input.badbit);    // throw for serious errors
  output.exceptions(output.failbit); // throw for any error

  try
  {
    // Lots of ways to copy: use std::copy, use a loop to read & write
    // The following is a little-known technique that is probably fastest.
    output << input.rdbuf();
    output.close();
    input.close();
  }
  catch (std::ios_base::failure const& ex)
  {
    std::cerr << "Can’t copy " << argv[1] << " to " << argv[2] << ": " <<
                 ex.what() << '\n';
    return EXIT_FAILURE;
  }
}
