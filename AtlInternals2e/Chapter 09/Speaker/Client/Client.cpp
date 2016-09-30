// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlbase.h>

#define SPEAKER_WITH_ATTRIBUTES

#ifdef SPEAKER_WITH_ATTRIBUTES
    #import "..\\Speaker\\Debug\\Speaker.dll" no_namespace, named_guids, raw_interfaces_only
    #define DEMAGOGUE_CLASS CDemagogue
    #define EARPOLITIC_CLASS CEarPolitic
#else
    #define DEMAGOGUE_CLASS Demagogue
    #define EARPOLITIC_CLASS EarPolitic
    #import "..\\SpeakerNoAttr\\Debug\\SpeakerNoAttr.dll" no_namespace, named_guids, raw_interfaces_only
#endif // SPEAKER_WITH_ATTRIBUTES

int _tmain(int argc, _TCHAR* argv[])
{
    ::CoInitialize(NULL);
    {
#ifdef SPEAKER_WITH_ATTRIBUTES
        HRESULT hr = S_OK;
        CComPtr<ISpeaker> spSpeaker;
        hr = spSpeaker.CoCreateInstance(__uuidof(DEMAGOGUE_CLASS));
        CComQIPtr<INamedObject> spNamedObject = spSpeaker;
        spNamedObject->put_Name(L"Demagogue");
        CComPtr<IListener> spListener;
        hr = spListener.CoCreateInstance(__uuidof(EARPOLITIC_CLASS));
        spNamedObject = spListener;
        //spNamedObject->put_Name(L"EarPolitic");
        spListener->ListenTo(Defendant, spSpeaker);
        spListener->ListenTo(Plaintiff, spSpeaker);
        spSpeaker->put_Speech(L"Now is the time for all good men...");
        spSpeaker->put_Volume(50);
        spSpeaker->Speak();
#else
        HRESULT hr = S_OK;
        CComPtr<ISpeaker> spSpeaker;
        hr = spSpeaker.CoCreateInstance(__uuidof(Demagogue));
        CComQIPtr<INamedObject> spNamedObject = spSpeaker;
        spNamedObject->put_Name(L"Demagogue");
        CComPtr<IListener> spListener;
        hr = spListener.CoCreateInstance(__uuidof(EarPolitic));
        spNamedObject = spListener;
        spNamedObject->put_Name(L"EarPolitic");
        spListener->ListenTo(Defendant, spSpeaker);
        spListener->ListenTo(Plaintiff, spSpeaker);
        spSpeaker->put_Speech(L"Now is the time for all good men...");
        spSpeaker->put_Volume(50);
        spSpeaker->Speak();
#endif // SPEAKER_WITH_ATTRIBUTES
    }
    ::CoUninitialize();
    return 0;
}

