// Listing 66-10. Adding a Protected Constructor to the assignment_mixin Class Template
template<class T>
class assignment_mixin {
public:
   T& operator=(T rhs)
   {
      rhs.swap(static_cast<T&>(*this));
      return static_cast<T&>(*this);
   }
protected:
  assignment_mixin() {}
};
