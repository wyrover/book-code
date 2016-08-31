#pragma once

//////////////////////////////////////////////////////////////////////////////

class SharedMemNames
{
public:

    static wformat formatConsoleParams;
    static wformat formatInfo;
    static wformat formatCursorInfo;
    static wformat formatBuffer;
    static wformat formatCopyInfo;
    static wformat formatTextInfo;
    static wformat formatMouseEvent;
    static wformat formatNewConsoleSize;
    static wformat formatNewScrollPos;
    static wformat formatWatchdog;

};

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

wformat SharedMemNames::formatConsoleParams(L"Console2_params_%1%");
wformat SharedMemNames::formatInfo(L"Console2_consoleInfo_%1%");
wformat SharedMemNames::formatCursorInfo(L"Console2_cursorInfo_%1%");
wformat SharedMemNames::formatBuffer(L"Console2_consoleBuffer_%1%");
wformat SharedMemNames::formatCopyInfo(L"Console2_consoleCopyInfo_%1%");
wformat SharedMemNames::formatTextInfo(L"Console2_consoleTextInfo_%1%");
wformat SharedMemNames::formatMouseEvent(L"Console2_consoleMouseEvent_%1%");
wformat SharedMemNames::formatNewConsoleSize(L"Console2_newConsoleSize_%1%");
wformat SharedMemNames::formatNewScrollPos(L"Console2_newScrollPos_%1%");
wformat SharedMemNames::formatWatchdog(L"Local\\Console2_parentProcessExit_%1%");

//////////////////////////////////////////////////////////////////////////////
