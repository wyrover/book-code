// Listing 42-1. Rewriting generate_id to Use the Function Call Operator
#ifndef GENERATE_ID_HPP_
#define GENERATE_ID_HPP_

/// Class for generating a unique ID number.
class generate_id
{
public:
  generate_id() : counter_{0} {}
  long operator()();
private:
  short counter_;
  static short prefix_;
  static short const max_counter_{32767};
};

#endif
