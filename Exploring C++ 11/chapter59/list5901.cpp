// Listing 59-1. Defining the list Class Template
template<class T>
class list
{
public:
   list()
  : head_{nullptr}, tail_{nullptr}, size_{0}
  {}
  ~list() { clear(); }

  void clear();               ///< Erase all nodes in the list. Reset size to 0.
  void push_back(T const& x); ///< Add to tail.
  void pop_back();            ///< Erase from tail.
  // Other useful functions omitted for brevity...
private:
  class node
  {
  public:
    node(T const& key)
    : next_{nullptr}, prev_{nullptr}, value_{key}
    {}
    node* next_;
    node* prev_;
    T     value_;
  };

  node* head_;       ///< Start of the list
  node* tail_;       ///< End of the list
  std::size_t size_; ///< Number of nodes in the list
};
