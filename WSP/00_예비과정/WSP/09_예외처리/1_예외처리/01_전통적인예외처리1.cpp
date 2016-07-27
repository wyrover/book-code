/*
//─────────────────────────────────────
	예외(exception)
//─────────────────────────────────────
*/

//int arr[10];
//arr[100] = 10;

/*
class String
{
	char *s;
public:
	String()		// 생성자 : 객체가 생성될 때 호출 
	{
		s = new char[10];
	}
	~String()		// 소멸자 : 객체가 소멸될 때 호출 
	{
		delete [] s;
	}
	char  operator [] ( int n )
	{
		return s[n];
	}
};

String str;				// 객체 생성 
char ch = str[10] ;		// str.operator[](10);
*/


char String::operation[] (int i) const
{
	if((1 >=0) && (i < length))
		return buffer[i];
	// 매개변수로 넘어온 첨자가 문자열의 길이 범위를 넘어설경우 0번째 배열 요소를 반환
	// 에러에 속하는 부분 
	else
		return buffer[0];
}

// 1. 프로그램 종료
char String::operation[] (int i) const
{
	if((1 >=0) && (i < length))
		return buffer[i];
	exit(-1);
}


// 2. 에러를 나타내는 값 반환
char String::operation[] (int i) const
{
	if((1 >=0) && (i < length))
		return buffer[i];
	else
		return -1;
}

// 3. 에러가 발생했음을 알리고 값을 반환하고 프로그램은 에러 상태를 
//    그대로 남겨둔다.
  char String::operation[] (int i) const
{
	if((1 >=0) && (i < length))
		return buffer[i];
	else
	{
		errno = -1;
    	return buffer[i];
	}
}



// 4. 에러가 발생할 때 사용하도록 제공되는 함수를 호출한다.
  void SetError(int errno)
  {
	switch(errno)
	{
		case = -1;
			cout << "Use a correct subscript!!\n;
			break;
			...
	}
  }

  char String::operation[] (int i) const
{
	if((1 >=0) && (i < length))
		return buffer[i];
	SetError(-1);
}


