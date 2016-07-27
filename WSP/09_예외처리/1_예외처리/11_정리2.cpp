#include <iostream>
using namespace std;


class StackException
{
public:
	enum TYPE {STACK_OVERFLOW, STACK_UNDERFLOW};
	TYPE type;				
	StackException(TYPE e):type(e) {}
	TYPE GetType() { return type; }  	
	void Delete() {delete this; }
};


class Stack
{
public:
	int count;	
	Stack() : count(0){}	
	void Push(){ count++;}
	void Pop() throw(StackException*)
	{ 
		if (count <= 0)
		{		throw new StackException(StackException::STACK_UNDERFLOW);
		}
		count--;		
	}	
};


void main()
{
	try
	{	
		Stack s;		
		s.Push();			
		s.Pop();
	}catch(StackException* e)	
	{
		if (e->GetType() == StackException::STACK_UNDERFLOW)
		{
			cout << "스택이 비어 있습니다." << endl;
		}
		e->Delete(); // 메모리 해제	
	}	
}
