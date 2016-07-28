// Listing 56-1. Header That Declares a Trivial Debugging Function
#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <string>

/** @brief Write a debug message to the file @c "debug.txt"
 * @param msg The message to write
 */
void debug(std::string const& msg);

#endif
