#ifndef __LOG__H__
#define __LOG__H__

#include "drivergenius/utils/log_impl.h"
	

#ifdef DISABLE_LOG
	#define LOG_ENTER() 
	#define LOG_LEAVE() 
	#define LOG_TEST() 
	#define LOG_ENABLE(bval)
	#define LOG_LEVEL(level, msg) 
	#define LOG_INIT_STRM()
	#define LOG_INIT_FILE()
#else
	#define LOG_ENTER()				{tlog::log_enter();} 
	#define LOG_LEAVE()				{tlog::log_leave();} 
	#define LOG_TEST()				{tlog::log_test();} 
	#define LOG_ENABLE(bval)		{tlog::log_enable(bval);}
	#define LOG_LEVEL(level, msg)	{if(tlog::log_enable()) {tlog::log_level(level, msg);}}
	#define LOG_INIT_STRM()			{tlog::init_stream_log();}
	#define LOG_INIT_FILE()			{tlog::init_file_log();}
#endif

#define LOG_NORMAL(msg)		LOG_LEVEL(tlog::normal, msg)
#define LOG_NOTICE(msg)		LOG_LEVEL(tlog::notification, msg)
#define LOG_WARNING(msg)	LOG_LEVEL(tlog::warning, msg)
#define LOG_ERROR(msg)		LOG_LEVEL(tlog::error, msg)
#define LOG_CRITICAL(msg)	LOG_LEVEL(tlog::critical, msg)


//#if defined(DEBUG) || defined(_DEBUG)


#endif