// Listing 64-12. Fixing the Second Mystery Program
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

/** Return the index of a value in a range.
 * Look for the first occurrence of @p value in the range
 * [<tt>first</tt>, <tt>last</tt>), and return the zero-based
 * index or -1 if @p value is not found.
 * @param first The start of the range to search
 * @param last One past the end of the range to search
 * @param value The value to search for
 * @return [0, size), such that size == last-first, or -1
 */
template<class InputIter>
typename std::iterator_traits<InputIter>::difference_type
index_of(InputIter first, InputIter last,
         typename std::iterator_traits<InputIter>::value_type const& value)
{
   InputIter iter{std::find(first, last, value)};
   if (iter == last)
      return -1;
   else
      return std::distance(first, iter);
}

/** Determine whether the first occurrence of a value in a container is
 * in the last position in the container.
 * @param container Any standard container
 * @param value The value to search for.
 * @return true if @p value is at the last position,
 *         or false if @p value is not found or at any other position.
 */
template<class T>
bool is_last(T const& container, typename T::value_type const& value)
{
   typename T::difference_type
        pos(index_of(container.begin(), container.end(), value));
  auto last_index(static_cast<typename T::difference_type>(container.size() - 1));
   return pos != -1 and pos == last_index;
}

int main()
{
   std::vector<int> data{};
   if (is_last(data, 10))
      std::cout << "10 is the last item in data\n";
}
