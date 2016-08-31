/*!
* \file cactus_mongoose.h
* \brief mongoose http服务器嵌入代理
* 
* mongoose http服务器嵌入代理
* 
* \author wangyang 
* \date 2015/02/02 
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_MONGOOSE_H__
#define __CACTUS_CACTUS_MONGOOSE_H__
#include <cactus/cactus.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <cactus/cactus_pattern.h>

namespace cactus
{

class CACTUS_API CMongooseImpl 
{
public:
    CMongooseImpl();
    ~CMongooseImpl();
public:
    bool init_server();
    bool unit_server();
    void run();    
protected:
    static long ref_;
    
private:    
    void*   server_;  
    int     signal_received_;
    boost::shared_ptr<boost::thread> thrd_;
};

typedef cactus::Singleton<CMongooseImpl> CMongoose;

} // namespace cactus

#endif // __CACTUS_CACTUS_MONGOOSE_H__
