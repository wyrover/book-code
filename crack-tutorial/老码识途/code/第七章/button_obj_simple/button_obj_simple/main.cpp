#include "Button.h"
#include "Form.h"
#include "GlobalResource.h"

class MyListener2 : public ButtonListener
{
public:
	virtual void onLeftButtonUp(Button * button)
	{		
		button->setCaption("change");	
	}
};

class MyForm : public Form
{
private:
	Button * m_button;
public:
	~MyForm(){
		delete m_button;
	}

	MyForm(): Form(false, TEXT("test")){
		m_button = new Button(0, 0, 100, 30, "hello", this->getHandle());
		m_button->setListener(new MyListener2());
	}
};

class MyListener : public ButtonListener
{
public:
	virtual void onLeftButtonUp(Button * button)
	{
		MessageBoxA(NULL, button->getCaption(),NULL, NULL);		
		new MyForm();
	}
};



int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	initApp(hInstance);
	MSG          msg ;	
	Form * form = new Form(true, TEXT("hello"));
	HWND parent = form->getHandle();	
	Button * btn = new Button(10, 20, 100, 30, "button1", parent);	
	MyListener * bListener = new MyListener();
	btn->setListener(bListener);
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}
