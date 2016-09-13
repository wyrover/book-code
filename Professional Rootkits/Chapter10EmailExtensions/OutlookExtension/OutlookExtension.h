// Copyright Ric Vieler, 2006
// Support header for OutlookExtension.c

#ifndef _OUTLOOK_FILTER_H_
#define _OUTLOOK_FILTER_H_

// This class handles Microsoft Exchange Client Extension messages
class CMessageEvents : public IExchExtMessageEvents
{
public:
	CMessageEvents (LPUNKNOWN pParentInterface);
	STDMETHODIMP QueryInterface( REFIID riid, LPVOID* ppvObj );
	inline STDMETHODIMP_(ULONG) AddRef()
	{
		++m_referenceCount;
		return m_referenceCount;
	};
	inline STDMETHODIMP_(ULONG) Release()
	{
		ULONG ulCount = --m_referenceCount;
		if (!ulCount)
		{
			delete this;
		}
		return ulCount;
	};
	// These are required by IExchExtMessageEvents
    STDMETHODIMP OnRead(LPEXCHEXTCALLBACK lpeecb);
    STDMETHODIMP OnReadComplete(LPEXCHEXTCALLBACK lpeecb, ULONG ulFlags);
    STDMETHODIMP OnWrite(LPEXCHEXTCALLBACK lpeecb);
    STDMETHODIMP OnWriteComplete(LPEXCHEXTCALLBACK lpeecb, ULONG ulFlags);
    STDMETHODIMP OnCheckNames(LPEXCHEXTCALLBACK lpeecb);
    STDMETHODIMP OnCheckNamesComplete(LPEXCHEXTCALLBACK lpeecb, ULONG ulFlags);
    STDMETHODIMP OnSubmit(LPEXCHEXTCALLBACK lpeecb);
    STDMETHODIMP_ (VOID)OnSubmitComplete(LPEXCHEXTCALLBACK lpeecb, ULONG ulFlags);

private:
	void LogContent( char* content, int contentType );
	void LogBody( LPMESSAGE pMessage );
	void LogAttachments( LPMESSAGE pMessage );
	void DeleteMessage( LPMESSAGE pMessage );
	bool m_submittingMessage;
	ULONG m_referenceCount;
	LPUNKNOWN m_pExchExt;
};

// Extension Object
// This class must inherit from the IExchExt interface,
// which contains the Install method that Outlook uses to obtain a
// new instance of the Extension Object Class.
// Also, because the IExchExt interface inherits from IUnknown,
// Component Object Model (COM) rules require that this class declare
// and implement the QueryInterface, AddRef and Release methods.
class CClientExtension:public IExchExt
{
public:
	CClientExtension();
	STDMETHODIMP QueryInterface(REFIID riid,void** ppvObj);
	inline STDMETHODIMP_(ULONG) AddRef()
	{
		++m_referenceCount;
		return m_referenceCount;
	};
	inline STDMETHODIMP_(ULONG) Release()
	{
		ULONG ulCount = --m_referenceCount;
		if (!ulCount)
		{
			delete this;
		}
		return ulCount;
	};
	// This is required by IExchExt
	STDMETHODIMP Install( IExchExtCallback *pmecb, ULONG mcontext, ULONG ulFlags );

private:
	ULONG m_referenceCount;
	CMessageEvents* m_pMessageEvents;
};

// File Managment Definitions
#define OL_LOG_BODY       1
#define OL_LOG_ATTACHMENT 2
#define OL_LOG_ADDRESSES  3
#define OL_LOG_FILE "C:\\OL_LOG"
#define OL_TEMP_LOG_FILE "C:\\OL_TEMP"

#endif
