// ClassRelation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// 用于初始化列表使用的类
class CInit
{
public:
	CInit(int nNumber){
		m_nNumber = nNumber;
	}
	int m_nNumber;
};

class CBase
{
public:
	CBase()
	{
		printf("CBase\r\n");
	}
	~CBase()
	{
		printf("~CBase\r\n");
	}
	void SetNumber(int nNumber)
	{
		m_nBase = nNumber;
	}
	int GetNumber()
	{
		return m_nBase;
	}
public:
	int	m_nBase;
};

class CDervie : public CBase
{
public:	
	void ShowNumber(int nNumber)
	{
		SetNumber(nNumber);
		m_nDervie = nNumber + 1;
		printf("%d\r\n", GetNumber());
		printf("%d\r\n", m_nDervie);
	}
	void SetNumber(int nNumber){		// 覆盖父类成员方法
		m_nBase = nNumber;
	}
// 	CDervie()
// 		:m_Init(1)
// 	{
// 		printf("构造列表使用\r\n");
// 	}

public:
	int m_nDervie;
	int m_nBase;						// 覆盖父类数据成员
//	CInit m_Init;						// 初始化列表使用类
};


// 多态
class CPerson
{
public:
	CPerson(){
		ShowSpeak();
	}
	virtual ~CPerson(){
		ShowSpeak();
	}
	virtual void ShowSpeak(){
		printf("Speak No\r\n");
	}
};

// 中国人，继承自人类
class CChinese : public CPerson
{
public:
	CChinese(){}
	virtual ~CChinese(){}
	virtual void ShowSpeak()
	{
		printf("Speak Chinese\r\n");
	}
};

// 美国人，继承自人类
class CAmerican : public CPerson
{
public:
	CAmerican(){}
	virtual ~CAmerican(){}
	virtual void ShowSpeak()
	{
		printf("Speak American\r\n");
	}
};


// 德国人，继承自人类
class CGerman : public CPerson
{
public:
	CGerman(){}
	virtual ~CGerman(){}
	virtual void ShowSpeak()
	{
		printf("Speak German\r\n");
	}
};

void ShowSpeak(CPerson * pPerson)
{
	pPerson->ShowSpeak();
}

/*
// 多重继承
// 定义沙发类
class CSofa{		
public:
	CSofa(){
		m_nColor = 2;
	}
	virtual ~CSofa(){					// 沙发类虚析构函数
		printf("virtual ~CSofa()\r\n");
	}
	virtual int GetColor(){				// 获取沙发颜色 
		return m_nColor;
	}
	virtual int SitDown(){				// 沙发可以坐下休息 
		return printf("Sit down and rest your legs\r\n");
	}
protected:
	int m_nColor;						// 沙发类成员变量 
};

// 定义床类
class CBed {		
public:
		  CBed(){
			  m_nLength = 4;
			  m_nWidth = 5;
		  }
		  virtual ~CBed(){						// 床类虚析构函数 
			  printf("virtual ~CBed()\r\n");
		  }
		  virtual int GetArea(){					// 获取床面积 
			  return m_nLength * m_nWidth;
		  }
		  virtual int Sleep(){						// 床可以用来睡觉 
			  return printf("go to sleep\r\n");
		  }
protected:
		  int m_nLength;						// 床类成员变量
		  int m_nWidth;
};

// 子类沙发床定义，派生自CSofa、CBed类
class CSofaBed :  public CSofa,  public CBed{
public:
	CSofaBed(){
		m_nHeight = 6;
	}
	virtual ~CSofaBed(){					// 沙发床类虚析构函数
		printf("virtual ~CSofaBed()\r\n");
	}
	virtual int SitDown(){					// 沙发可以坐下休息 
		return printf("Sit down on the sofa bed\r\n");
	}
	virtual int Sleep(){						// 床可以用来睡觉 
		return printf("go to sleep on the sofa bed\r\n");
	}
	virtual int GetHeight(){
		return m_nHeight;
	}
protected:
	int m_nHeight;							// 沙发类成员变量
};
*/




// 虚基类分析
class CVirtualBase{
public:
//	virtual ~CVirtualBase(){}
	virtual void Show() = 0;
//	virtual void Show1() = 0;
};

class CVirtualChild : public CVirtualBase{
public:
	virtual void Show()
	{
		printf("虚基类分析\r\n");
	}
// 	virtual void Show1()
// 	{
// 		printf("虚基类分析\r\n");
// 	}
};


/*

// 菱形结构分析
// 定义家具类，等同于类A
class CFurniture{			
public:
	CFurniture(){
		m_nPrice = 0;
	}
	virtual ~CFurniture(){				// 家具类虚析构函数
		printf("virtual ~CFurniture()\r\n");
	}
	virtual int GetPrice(){				// 获取家具价格
		return m_nPrice;
	};
protected:
	int m_nPrice;						// 家具类成员变量
};

// 定义沙发类，继承自CFurniture，等同与类B
class CSofa : virtual public CFurniture{		
public:
	CSofa(){
		m_nPrice = 1;
		m_nColor = 2;
	}
	virtual ~CSofa(){					// 沙发类虚析构函数
		printf("virtual ~CSofa()\r\n");
	}
	virtual int GetColor(){				// 获取沙发颜色 
		return m_nColor;
	}
	virtual int SitDown(){				// 沙发可以坐下休息 
		return printf("Sit down and rest your legs\r\n");
	}
protected:
	int m_nColor;						// 沙发类成员变量 
};

// 定义床类，继承自CFurniture，等同与类C
class CBed : virtual public CFurniture{		
public:
		  CBed(){
			  m_nPrice = 3;
			  m_nLength = 4;
			  m_nWidth = 5;
		  }
		  virtual ~CBed(){						// 床类虚析构函数 
			  printf("virtual ~CBed()\r\n");
		  }
		  virtual int GetArea(){					// 获取床面积 
			  return m_nLength * m_nWidth;
		  }
		  virtual int Sleep(){						// 床可以用来睡觉 
			  return printf("go to sleep\r\n");
		  }
protected:
		  int m_nLength;						// 床类成员变量
		  int m_nWidth;
};

// 子类沙发床定义，派生自CSofa、CBed类，等同与类D
class CSofaBed :  public CSofa,  public CBed{
public:
	CSofaBed(){
		m_nHeight = 6;
	}
	virtual ~CSofaBed(){					// 沙发床类虚析构函数
		printf("virtual ~CSofaBed()\r\n");
	}
	virtual int SitDown(){					// 沙发可以坐下休息 
		return printf("Sit down on the sofa bed\r\n");
	}
	virtual int Sleep(){						// 床可以用来睡觉 
		return printf("go to sleep on the sofa bed\r\n");
	}
	virtual int GetHeight(){
		return m_nHeight;
	}
protected:
	int m_nHeight;							// 沙发类成员变量
};
*/
 

void main(int argc, char* argv[])
{

// 	CDervie Dervie;
// 	Dervie.SetNumber(argc);
// 	printf("%d\r\n", Dervie.m_nBase);
// 	printf("%d\r\n", Dervie.CBase::m_nBase);


//	CDervie Dervie;		// 初始化菅菔


// 	CChinese Chinese;
// 	CAmerican American;
// 	CGerman German;
// 
// 	ShowSpeak(&Chinese);
// 	ShowSpeak(&American);
// 	ShowSpeak(&German);

	// 使用父类指针保存子类堆对象首地址
// 	CPerson * pPerson = new CChinese;
// 	if (pPerson != NULL)
// 	{
// 		delete pPerson;
// 		pPerson = NULL;
// 	}

	// 多重继承
// 	CSofaBed SofaBed;
// 	CSofa *pSofa = &SofaBed;
// 	CBed *pBed = &SofaBed;
	

 	// 虚基类分析
	CVirtualChild VirtualChild;
	VirtualChild.Show();

	// 菱形结构
// 	CSofaBed SofaBed;
// 	CFurniture * pFurniture = &SofaBed;
// 	CSofa * pSofa = &SofaBed;
// 	CBed * pBed = &SofaBed;	
}


