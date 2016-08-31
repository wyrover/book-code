/*!
* \file cactus_mongoose.cpp
* \brief mongoose http服务器嵌入实现
*
* mongoose http服务器嵌入实现
*
* \author wangyang
* \date 2015/02/02
* \version 1.0
*/

#include <cactus/cactus_mongoose.h>
#include <cactus/cactus_pattern.h>
#include <cactus/mongoose/mongoose.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <windows.h>


namespace cactus
{

long CMongooseImpl::ref_ = 0;

CMongooseImpl::CMongooseImpl() : signal_received_(0)
{

}

CMongooseImpl::~CMongooseImpl()
{

}


bool CMongooseImpl::init_server()
{
    if (1 == ::InterlockedIncrement(&CMongooseImpl::ref_)) {
        server_ = mg_create_server(NULL, NULL);
        struct mg_server* pserver = (struct mg_server *)server_;
        mg_set_option(pserver, "document_root", ".");      // Serve current directory
        mg_set_option(pserver, "listening_port", "8080");  // Open port 8080
        boost::function0<void> f = boost::bind(&CMongooseImpl::run, this);
        thrd_ = boost::shared_ptr<boost::thread>(new boost::thread(f));
    }
    return true;
}

bool CMongooseImpl::unit_server()
{
    if (0 == ::InterlockedDecrement(&CMongooseImpl::ref_)) {
        signal_received_ = 1;
        thrd_->join();
        struct mg_server* pserver = (struct mg_server *)server_;
        mg_destroy_server(&pserver);
    }
    return true;
}

void CMongooseImpl::run()
{
    struct mg_server* pserver = (struct mg_server *)server_;

    while (signal_received_ == 0) {
        mg_poll_server(pserver, 1000);   // Infinite loop, Ctrl-C to stop
    }
}



} // namespace cactus