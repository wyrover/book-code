#include "Button.h"
#include "Form.h"
#include "Edit.h"
#include "GlobalResource.h"

class MyForm;

class MyListener : public ButtonListener
{
public:
	MyListener(MyForm * form){
		m_form = form;
	}
	virtual void onLeftButtonUp(Button * button);
private:
	MyForm * m_form;
};


class MyForm : public Form
{
	friend class MyListener;
private:
	Button * m_button;
	Edit * m_edit;
public:
	~MyForm(){
		delete m_button;
		delete m_edit;
	}

	MyForm(): Form(true, TEXT("test")){
		m_button = new Button(0, 0, 100, 30, "hello", this->getHandle());
		m_button->setListener(new MyListener(this));
		m_edit = new Edit(0, 50, 100, 30, "edit1", this->getHandle());
	}
};


void MyListener::onLeftButtonUp(Button * button)
{	
	m_form->m_edit->setText("click it");
	button->setCaption(m_form->m_edit->getText());		
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	initApp(hInstance);
	MSG          msg ;	
	MyForm * form = new MyForm();	
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}
