/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
namespace MyLib
{
template <typename Cont>
inline
bool replace_key(Cont& c,
                 const typename Cont::key_type& old_key,
                 const typename Cont::key_type& new_key)
{
    typename Cont::iterator pos;
    pos = c.find(old_key);

    if (pos != c.end()) {
        // insert new element with value of old element
        c.insert(typename Cont::value_type(new_key,
                                           pos->second));
        // remove old element
        c.erase(pos);
        return true;
    } else {
        // key not found
        return false;
    }
}
}
