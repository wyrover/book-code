#pragma once

#include "resource.h"
#include <Windows.h>
#include <cassert>
//#include <string>

//from my altWinDirStat repo, macros_that_scare_small_children.h
#ifndef WDS_ASSERT_EXPECTED_STRING_FORMAT_FAILURE_HRESULT

//I don't think anything in strsafe actually ever returns STRSAFE_E_END_OF_FILE,
//so I use this after I've handled the other error conditions (STRSAFE_E_INSUFFICIENT_BUFFER, STRSAFE_E_INVALID_PARAMETER),
//to catch unexpected errors. NOTE that these are still handled by the calling function via SUCCESSS( ),
//but this macro helps catch the issue closer to the function that returned the unexpected value;
#define WDS_ASSERT_EXPECTED_STRING_FORMAT_FAILURE_HRESULT( res ) {                                                \
        static_assert( SUCCEEDED( S_OK ), "This macro depends on SUCCEEDED( S_OK ) returning true" );                 \
        static_assert( std::is_same<decltype( res ), const HRESULT>::value, "This macro depends on an HRESULT res" ); \
        assert( ( res ) != STRSAFE_E_END_OF_FILE );                                                                   \
        assert( FAILED( res ) );                                                                                      \
        assert( !SUCCEEDED( res ) );                                                                                  \
    }

#else
#error already defined!
#endif


//On returning E_FAIL, call GetLastError for details. That's not my idea!
_Success_(SUCCEEDED(return)) HRESULT CStyle_GetLastErrorAsFormattedMessage(_Out_writes_z_(strSize) _Pre_writable_size_(strSize) PWSTR psz_formatted_error, _In_range_(128, 32767) const rsize_t strSize, const DWORD error = GetLastError());
