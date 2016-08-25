class ObjectManager
{
public:
    template<typename T>
    T* gimmeAnObject();

    template<typename T>
    void gimmeAnObject(T*& p);
};

template<typename T>
T* ObjectManager::gimmeAnObject()
{
    return (new T);
}

template<typename T>
void ObjectManager::gimmeAnObject(T*& p)
{
    p = new T;
}

class X
{
    /* ... */
};
class Y
{
    /* ... */
};

int main()
{
    ObjectManager om;
    X* p1 = om.gimmeAnObject<X>();  // You have to specify the template
    Y* p2 = om.gimmeAnObject<Y>();  // parameter
    om.gimmeAnObject(p1);  // Not here, though, since the compiler can
    om.gimmeAnObject(p2);  // deduce T from the arguments
}