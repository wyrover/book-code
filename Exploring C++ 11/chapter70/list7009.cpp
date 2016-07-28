// Listing 70-9. One Way to Implement std::conditional
template<bool Condition, class IfTrue, class IfFalse>
struct conditional
{
    typedef IfFalse type;
};

template<class IfTrue, class IfFalse>
struct conditional<true, IfTrue, IfFalse>
{
   typedef IfTrue type;
};
