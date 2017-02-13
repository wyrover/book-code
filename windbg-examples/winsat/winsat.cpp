#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <atlbase.h>
#include <winsatcominterfacei.h>

#include <string>
#include <vector>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

void main(void)
{
    HRESULT hr = S_OK;
    
    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    CComPtr<IQueryRecentWinSATAssessment> pAssessment = nullptr;
    // Get an instance to the most recent formal assessmenet.
    hr = CoCreateInstance(__uuidof(CQueryWinSAT), nullptr, CLSCTX_INPROC_SERVER, 
                          __uuidof(IQueryRecentWinSATAssessment), (void**)&pAssessment);

    if (FAILED(hr))
    {
        wprintf(L"Failed to create an instance of IQueryRecentWinSATAssessment. Failed with 0x%x.\n", hr);
        return;
    }

    CComPtr<IProvideWinSATResultsInfo> pResults = nullptr;
    // Get the summary information for the assessment.
    hr = pAssessment->get_Info(&pResults);
    if (FAILED(hr))
    {
        wprintf(L"pAssessment->get_Info failed with 0x%x.\n", hr); 
        return;
    }

    WINSAT_ASSESSMENT_STATE state;
    // Get the state of the assessment. If the state is valid,
    // print the date of the assessment and the base score.
    hr = pResults->get_AssessmentState(&state);
    if (FAILED(hr))
    {
        wprintf(L"pResults->get_AssessmentState failed with 0x%x.\n", hr);
        return;
    }

    float baseScore = 0.0;
    VARIANT vAssessmentDate;
    SYSTEMTIME st;

    switch (state)
    {
    case WINSAT_ASSESSMENT_STATE_UNKNOWN:
        wprintf(L"The state of the assessment is unknown.\n");
        break;

    case WINSAT_ASSESSMENT_STATE_VALID:
    case WINSAT_ASSESSMENT_STATE_INCOHERENT_WITH_HARDWARE:
        hr = pResults->get_AssessmentDateTime(&vAssessmentDate);
        if (FAILED(hr))
        {
            wprintf(L"pResults->get_AssessmentDateTime failed with 0x%x.\n", hr);
            break;
        }

        VariantTimeToSystemTime(vAssessmentDate.dblVal, &st);
        wprintf(L"Assessment ran on %2d/%2d/%2d at %2d:%2d\n",
            st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute);

        hr = pResults->get_SystemRating(&baseScore);
        if (SUCCEEDED(hr))
        {
            wprintf(L"The base score for the computer is %.1f\n", baseScore);

            {
                typedef std::pair<WINSAT_ASSESSMENT_TYPE, std::wstring> WinSatAssessmentTypeToDesc;
                std::vector<WinSatAssessmentTypeToDesc> assessmentTypes;
                assessmentTypes.push_back(std::make_pair(WINSAT_ASSESSMENT_MEMORY, L"Memory"));
                assessmentTypes.push_back(std::make_pair(WINSAT_ASSESSMENT_CPU, L"CPU"));
                assessmentTypes.push_back(std::make_pair(WINSAT_ASSESSMENT_D3D, L"Direct3D"));
                assessmentTypes.push_back(std::make_pair(WINSAT_ASSESSMENT_GRAPHICS, L"Graphics"));
                assessmentTypes.push_back(std::make_pair(WINSAT_ASSESSMENT_DISK, L"Disk"));

                for(auto assessment : assessmentTypes)
                {
                    CComPtr<IProvideWinSATAssessmentInfo> info = nullptr;
                    hr = pResults->GetAssessmentInfo(assessment.first, &info);
                    float score = 0;
                    info->get_Score(&score);
                    wprintf(L"Score for %-8s --> %.1f\n", assessment.second.c_str(), score);
                }
            }


            if (WINSAT_ASSESSMENT_STATE_INCOHERENT_WITH_HARDWARE == state)
            {
                wprintf(L"Note that the hardware configuration of the computer has changed\n"
                    L"since the assessment was run; the score reflects the prior configuration.");
            }
        }
        else
        {
            wprintf(L"pResults->get_SystemRating failed with 0x%x.\n", hr);
        }
        break;

    case WINSAT_ASSESSMENT_STATE_NOT_AVAILABLE:
        wprintf(L"A formal assessment has not run on the computer.\n");
        break;

    case WINSAT_ASSESSMENT_STATE_INVALID:
        wprintf(L"The assessment data is not valid.\n");
        break;

    default:
        wprintf(L"The assessment state value is not valid.\n");
    }

    VariantClear(&vAssessmentDate);
    CoUninitialize();
}
