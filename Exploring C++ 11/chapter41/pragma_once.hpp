/* Another way to ensure a header is included only once
 * is to use #pragma once, as shown below. By definition,
 * #pragma is not portable. Compilers are free to do
 * whatever they way. A growing convention is that
 * #pragma once means to include the header only
 * once. When implemented, it is potentially more
 * efficient because the compiler doesn't even need
 * to open the file.
 * 
 * Because #pragma once is not portable, use #ifndef
 * guards, too.
 * 
 * I did not cover #pragma once in the book because
 * it is not part of the C++ standard. But it is
 * useful, and good to know about.
 */
#ifndef PRAGMA_ONCE_HPP_
#define PRAGMA_ONCE_HPP_

#pragma once

// put your declarations here

#endif
