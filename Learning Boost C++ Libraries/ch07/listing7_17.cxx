 1 template <typename T, size_t N>
 2 typename boost::disable_if<boost::is_pod<T>>::type
 3 copy(T (&lhs)[N], T (&rhs)[N])
 4 {
 5   for (size_t i = 0; i < N; ++i) {
 6     lhs[i] = rhs[i];
 7   }
 8 }

