/***
*crtmbox.c - CRT MessageBoxA wrapper.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Wrap MessageBoxA.
*
*******************************************************************************/

#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <awint.h>
#include <internal.h>

/***
*__crtMessageBox - call MessageBoxA dynamically.
*
*Purpose:
*       Avoid static link with user32.dll. Only load it when actually needed.
*
*Entry:
*       see MessageBoxA docs.
*
*Exit:
*       see MessageBoxA docs.
*
*Exceptions:
*
*******************************************************************************/
#ifdef _UNICODE
int __cdecl __crtMessageBoxW(
#else  /* _UNICODE */
int __cdecl __crtMessageBoxA(
#endif  /* _UNICODE */
        LPCTSTR lpText,
        LPCTSTR lpCaption,
        UINT uType
        )
{
        typedef int (APIENTRY *PFNMessageBox)(HWND, LPCTSTR, LPCTSTR, UINT);
        typedef HWND (APIENTRY *PFNGetActiveWindow)(void);
        typedef HWND (APIENTRY *PFNGetLastActivePopup)(HWND);
        typedef HWINSTA (APIENTRY *PFNGetProcessWindowStation)(void);
        typedef BOOL (APIENTRY *PFNGetUserObjectInformation)(HANDLE, int, PVOID, DWORD, LPDWORD);

        void *pfn = NULL;
        void *enull = _encoded_null();
        static PFNMessageBox pfnMessageBox = NULL;
        static PFNGetActiveWindow pfnGetActiveWindow = NULL;
        static PFNGetLastActivePopup pfnGetLastActivePopup = NULL;
        static PFNGetProcessWindowStation pfnGetProcessWindowStation = NULL;
        static PFNGetUserObjectInformation pfnGetUserObjectInformation = NULL;

        HWND hWndParent = NULL;
        BOOL fNonInteractive = FALSE;
        HWINSTA hwinsta;
        USEROBJECTFLAGS uof;
        DWORD nDummy;
        unsigned int osplatform = 0;
        unsigned int winmajor = 0;

        if (NULL == pfnMessageBox)
        {
            HMODULE hlib=LoadLibrary(_T("USER32.DLL"));
            if(hlib==NULL)
            {
                return 0;
            }

            if (NULL == (pfn =
#ifdef _UNICODE
                            GetProcAddress(hlib, "MessageBoxW")))
#else  /* _UNICODE */
                            GetProcAddress(hlib, "MessageBoxA")))
#endif  /* _UNICODE */
                return 0;

            pfnMessageBox = (PFNMessageBox) _encode_pointer(pfn);

            pfnGetActiveWindow = (PFNGetActiveWindow)
                _encode_pointer(GetProcAddress(hlib, "GetActiveWindow"));

            pfnGetLastActivePopup = (PFNGetLastActivePopup)
                _encode_pointer(GetProcAddress(hlib, "GetLastActivePopup"));

            _ERRCHECK(_get_osplatform(&osplatform));
            if (osplatform == VER_PLATFORM_WIN32_NT)
            {
                pfn =
#ifdef _UNICODE
                        GetProcAddress(hlib, "GetUserObjectInformationW");
#else  /* _UNICODE */
                        GetProcAddress(hlib, "GetUserObjectInformationA");
#endif  /* _UNICODE */

                pfnGetUserObjectInformation = (PFNGetUserObjectInformation) _encode_pointer(pfn);

                if (pfnGetUserObjectInformation != NULL)
                    pfnGetProcessWindowStation = (PFNGetProcessWindowStation)
                        _encode_pointer(GetProcAddress(hlib, "GetProcessWindowStation"));
            }
        }

        /*
         * If the current process isn't attached to a visible WindowStation,
         * (e.g. a non-interactive service), then we need to set the
         * MB_SERVICE_NOTIFICATION flag, else the message box will be
         * invisible, hanging the program.
         *
         * This check only applies to Windows NT-based systems (for which we
         * retrieved the address of GetProcessWindowStation above).
         */

        if (pfnGetProcessWindowStation != enull && pfnGetUserObjectInformation != enull)
        {
            if (NULL == (hwinsta = ((PFNGetProcessWindowStation) _decode_pointer(pfnGetProcessWindowStation))()) ||
                !((PFNGetUserObjectInformation) _decode_pointer(pfnGetUserObjectInformation))
                    (hwinsta, UOI_FLAGS, &uof, sizeof(uof), &nDummy) ||
                (uof.dwFlags & WSF_VISIBLE) == 0)
            {
                fNonInteractive = TRUE;
            }
        }

        if (fNonInteractive)
        {
            _ERRCHECK(_get_winmajor(&winmajor));
            if (winmajor >= 4)
                uType |= MB_SERVICE_NOTIFICATION;
            else
                uType |= MB_SERVICE_NOTIFICATION_NT3X;
        }
        else
        {
            if (pfnGetActiveWindow != enull)
                hWndParent = ((PFNGetActiveWindow) _decode_pointer(pfnGetActiveWindow))();

            if (hWndParent != NULL && pfnGetLastActivePopup != enull)
                hWndParent = ((PFNGetLastActivePopup) _decode_pointer(pfnGetLastActivePopup))(hWndParent);
        }

        return ((PFNMessageBox) _decode_pointer(pfnMessageBox))(hWndParent, lpText, lpCaption, uType);
}
