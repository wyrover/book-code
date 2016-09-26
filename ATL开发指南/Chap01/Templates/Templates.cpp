//
// Templates.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

//#define NO_TEMPLATES

#ifdef NO_TEMPLATES
class StackInt
{
public:
    StackInt()
    {
        m_sSize = 100;
        m_data = new int[ m_sSize ];
        m_sPos = 0;
    }
    StackInt(short sSize)
    {
        m_sSize = sSize;
        m_data = new int[ m_sSize ];
        m_sPos = 0;
    }
    ~StackInt() {}

    void     Push(int iValue);
    int      Pop();

    bool IsEmpty()
    {
        return (m_sPos == 0);
    };
    bool HasElements()
    {
        return (m_sPos != 0);
    }
    bool IsFull()
    {
        return (m_sPos == m_sSize);
    }

private:
    short m_sSize;
    int* m_data;
    short m_sPos;
};

void StackInt::Push(int iValue)
{
    m_data[ m_sPos++ ] = iValue;
}

int StackInt::Pop()
{
    return m_data[ --m_sPos ];
}

#elif NO_SIZE_PARAMETER
template <class T>
class Stack
{
public:
    Stack()
    {
        m_sPos = 0;
    }
    ~Stack() {}

    void Push(T value);
    T    Pop();

    bool IsEmpty()
    {
        return (m_sPos == 0);
    };
    bool HasElements()
    {
        return (m_sPos != 0);
    }
    bool IsFull()
    {
        return (m_sPos == 100);
    }

private:
    T m_data[100];
    short m_sPos;
};

template <class T>
void Stack<T>::Push(T value)
{
    m_data[m_sPos++] = value;
}

template <class T>
T Stack<T>::Pop()
{
    return m_data[--m_sPos];
}
#else
template <class T, short sSize = 100>
class Stack
{
public:
    Stack()
    {
        m_sPos = 0;
    }
    ~Stack() {}

    void Push(T value);
    T    Pop();

    bool IsEmpty()
    {
        return (m_sPos == 0);
    };
    bool HasElements()
    {
        return (m_sPos != 0);
    }
    bool IsFull()
    {
        return (m_sPos == sSize);
    }
    long GetSize()
    {
        return sSize;
    }

private:
    T m_data[ sSize ];
    short m_sPos;
};

template <class T, short sSize = 100>
void Stack<T>::Push(T value)
{
    m_data[ m_sPos++ ] = value;
}

template <class T, short sSize = 100>
T Stack<T>::Pop()
{
    return m_data[ --m_sPos ];
}
#endif


template <class Type>
Type MAX(Type a, Type b)
{
    if (a > b)
        return a;
    else
        return b;
}

int main(int argc, char *argv[])
{
    // Function templates
    int    iMax = MAX<int>(10, 12);
    cout << "MAX( 10, 12 ) is " << iMax << endl;
    long   lMax = MAX<long>(10, 12);
    cout << "MAX( 10, 12 ) is " << lMax << endl;
    double dMax = MAX<double>(10.0, 12.0);
    cout << "MAX( 10.0, 12.0 ) is " << dMax << endl;
#ifdef NO_TEMPLATES
    StackInt stack(10);
    stack.Push(100);
    stack.Push(200);
    stack.Push(300);

    while (stack.HasElements()) {
        cout << stack.Pop() << endl;
    }

#else
    Stack< int > intStack;
    Stack< int, 10 > intStack10;
    // A dynamically created stack
    Stack<long>* plongStack = new Stack<long>;
    plongStack->Push(1000);
    plongStack->Push(2000);
    delete plongStack;
    intStack.Push(100);
    intStack.Push(200);
    intStack.Push(300);

    while (intStack.HasElements()) {
        cout << intStack.Pop() << endl;
    }

    Stack< double > doubleStack;
    Stack< double, 20 > doubleStack20;
    cout << "doubleStack20 size is " << doubleStack20.GetSize() << endl;
    doubleStack.Push(1.1);
    doubleStack.Push(2.2);
    doubleStack.Push(3.3);
    doubleStack.Push(4.4);

    while (doubleStack.HasElements()) {
        cout << doubleStack.Pop() << endl;
    }

#endif
    return 0;
}