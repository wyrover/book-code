#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/thread_pool.h"
#include <Windows.h>
#include <iostream>

struct test_inc {
    void do_job(std::string str)
    {
        CHAR msg[100] = {0};
        sprintf(msg, "%d - %s", ::GetCurrentThreadId(), str.c_str());
        TRACE_SEND_A(msg);
        ::Sleep(100);
    }
};

TEST_CASE("Test for common/thread_pool", "[common/thread_pool]")
{
    SECTION("test_verify_signature", "[common/thread_pool]") {
        thread_pool tp(10);
        test_inc t;
        std::vector<std::string> ids;

        for (int i = 0; i <= 1000; ++i) {
            char id_msg[100] = {0};
            sprintf(id_msg, "test - %d", i);
            ids.push_back(id_msg);
        }

        std::vector<std::string>::iterator it;

        for (it = ids.begin(); it < ids.end(); ++it) {
            tp.enqueue(boost::bind(&test_inc::do_job, &t, *it));
        }

        ::Sleep(10000);
    }
}

