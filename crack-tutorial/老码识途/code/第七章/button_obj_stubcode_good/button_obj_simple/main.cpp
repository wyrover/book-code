#include "Button.h"
#include "Form.h"
#include "Edit.h"
#include "GlobalResource.h"

class MyForm;

class FormListener : public ControlListener
{
public:
	FormListener(MyForm * form){
		m_form = form;		
	}
	virtual void onLeftButtonUp(Control * ctrl);
protected:
	MyForm * m_form;	
};

class MyListener : public FormListener
{
public:
	MyListener(MyForm * form): FormListener(form){
		m_visible = true;
		m_x = 200;
	}
	virtual void onLeftButtonUp(Control * ctrl);
private:
	bool m_visible;
	int m_x;
};


class MyForm : public Form
{
	friend class MyListener;
private:
	Button * m_button1;
	Button * m_button2;
	Edit * m_edit;
public:
	~MyForm(){
		delete m_button1;
		delete m_button2;
		delete m_edit;
	}

	MyForm(): Form(true, "test"){
		m_button1 = new Button(0, 0, 100, 30, "hello1", this->getHandle());
		m_button1->setListener(new MyListener(this));
		m_button2 = new Button(300, 0, 100, 30, "hello2", this->getHandle());
		m_edit = new Edit(0, 50, 100, 30, "edit1", this->getHandle());
		setListener(new FormListener(this));
		m_button1->show();
		m_button2->show();
		m_edit->show();
	}
};

void FormListener::onLeftButtonUp(Control * ctrl)
{	
	MessageBoxA(NULL, "form left up", NULL, NULL);	
}

void MyListener::onLeftButtonUp(Control * ctrl)
{	
	m_form->m_edit->setCaption("click it");
	ctrl->setCaption(m_form->m_edit->getCaption());		
	if (m_visible)	
		m_form->m_button2->hide();	
	else
	{
		m_form->m_button2->setX(m_x);
		m_form->m_button2->show();
	}
	m_visible = !m_visible;
	m_x += 5;	
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	Application::initApp(hInstance);
	MyForm * form = new MyForm();	
	form->show();
	return Application::handleMessage();
}
