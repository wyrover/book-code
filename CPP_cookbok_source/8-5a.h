// Static.h
class OneStatic
{
public:
    int getCount()
    {
        return count;
    }
    OneStatic();
protected:
    static int count;
};