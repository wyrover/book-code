// counting.h
// Definition of counting_iterator.
#include <iostream.h>
#include <bstring.h>

template <class ForwardIterator, class T>
class counting_iterator {
private:
    ForwardIterator current;
    int plus_count;
    string name;
    int version;
public:
    counting_iterator(ForwardIterator first,
                      const string& n)
       : current(first),
         plus_count(0),
         name(n),
         version(1) { }

    counting_iterator(const
            counting_iterator<ForwardIterator, T>& other) 
       : current(other.current),
         plus_count(other.plus_count),
         name(other.name),
         version(other.version + 1) 
      // copy constructor       
    { 
            cout << "copying " << name 
                 << ", new version is "
                 << version << endl;
    }

    T& operator*() 
      // dereference
    {
        return *current;
    }

    bool operator==(const
       counting_iterator<ForwardIterator, T>& other) const 
      // test for equality
    {
        return current == other.current;
    }

    bool operator!=(const
       counting_iterator<ForwardIterator, T>& other) 
          const 
      // test for inequality
    {
        return current != other.current;
    }

    counting_iterator<ForwardIterator, T>& operator++() 
     // prefix ++
    {                            
        ++current;
        ++plus_count;
        return *this;
    }

    counting_iterator<ForwardIterator, T> operator++(int) 
      // postfix ++
    {   
        counting_iterator<ForwardIterator, T> tmp = *this;
        ++(*this);
        return tmp;
    }

    void report() const 
      // Display statistics on standard 
      // output stream.
    {
        cout << "Iterator " << name 
             << ", v. " << version 
             << ", reporting  " << plus_count 
             << "  ++ operations " << endl;
    }
};
