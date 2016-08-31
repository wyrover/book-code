#include <catch.hpp>
#include <tracetool/tracetool.h>
#include <Windows.h>
#include <iostream>
#include "SharedMemory.h"
#include "Structures.h"

TEST_CASE("Test for shared_memory", "[common/shared_memory]")
{
    SECTION("test_shared_memory_server", "[common/shared_memory]") {
        SharedMemory<ConsoleParams> consoleParams;
        consoleParams.Create(L"Console2_params", 1, syncObjBoth, L"");
        {
            SharedMemoryLock memLock((SharedMemory<ConsoleParams>&)consoleParams);
            consoleParams->dwRows       = 25;
            consoleParams->dwColumns    = 80;
        }
        ::WaitForSingleObject(consoleParams.GetReqEvent(), 10000);
        //consoleParams.SetRespEvent();
        TRACE_SEND_A("finish");
    }
    SECTION("test_shared_memory_client", "[common/shared_memory]") {
        SharedMemory<ConsoleParams> consoleParams2;
        consoleParams2.Open(L"Console2_params", syncObjRequest);
        // ·¢ËÍ¶ÁÈ¡
        consoleParams2.SetReqEvent();
        TRACE_SEND_FORMAT("%d", consoleParams2->dwRows);
    }
}

