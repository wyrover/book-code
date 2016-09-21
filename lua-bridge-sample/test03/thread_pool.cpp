#include "thread_pool.h"
//-------------------------------------------------------------------------------
thread_pool::thread_pool( int thread_count ) 
: _service(new boost::asio::io_service())
, _base_work(new boost::asio::io_service::work(*_service))

{
	for(int i=0 ; i<thread_count ; i++){
		_threads.create_thread( boost::bind(&thread_pool::thread_func, this) );
	}
}
//-------------------------------------------------------------------------------
thread_pool::~thread_pool( void )
{
	_service->stop(); // stop all jobs
	_threads.join_all(); // let threads finish their current jobs
}
//-------------------------------------------------------------------------------
void thread_pool::thread_func( const thread_pool* pool )
{
	pool->execute_orders();
}
//-------------------------------------------------------------------------------
void thread_pool::execute_orders( void ) const
{
	_service->run();
}

