#ifndef __LOG_IMPL_H__
#define __LOG_IMPL_H__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/log/sources/severity_logger.hpp>			//wseverity_logger_mt
#include <boost/log/sources/global_logger_storage.hpp>		//BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT
#include <boost/log/sources/logger.hpp>						//boost::log::sources::logger_mt
#include <boost/format.hpp>
#include <boost/logic/tribool.hpp>

namespace tlog{

template<typename C>
const C * ChooseCW(const char * c, const wchar_t * w);

#define CW(CharT, STR) ChooseCW<CharT>(STR, L##STR)

#define LOG_LOCALE "Chinese"

enum severity_level{
	normal,
	notification,
	warning,
	error,
	critical
};

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, severity_level level)
{
	// 这里的每一个值要与severity_level enum对应
	static const CharT* const strings[] ={
		CW(CharT, " NORMAL"),
		CW(CharT, " NOTICE"),
		CW(CharT, "WARNING"),
		CW(CharT, " ERROR "),
		CW(CharT, "CRITICAL"),
	};

	if (static_cast< std::size_t >(level) < (sizeof(strings) / sizeof(*strings)))
		strm << strings[level];
	else
		strm << static_cast< int >(level);
	return strm;
}

enum {
	LOG_RECORDS_TO_WRITE = 10 ,
	THREAD_COUNT = 2,
	MAX_FILE_SIZE = 1 * 1024 * 1024,
	MAX_LOG_SIZE = 16 * 1024 * 1024,
	MAX_FREE_SPACE = 100 * 1024 * 1024
};

typedef boost::log::sources::severity_logger_mt<severity_level> severity_logger_t;
typedef boost::log::sources::wseverity_logger_mt<severity_level> wseverity_logger_t;
typedef boost::log::sources::logger_mt my_logger_t;
typedef boost::log::sources::wlogger_mt my_wlogger_t;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_slog, my_logger_t)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_wlog, my_wlogger_t)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_slogger, severity_logger_t)
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_wslogger, wseverity_logger_t)


void log_test();
bool log_enable(boost::tribool tbval = boost::indeterminate);

int init_stream_log();
int init_file_log();

void log_enter();
void log_leave();

void log_msg(const char* logmsg);
void log_msg(const wchar_t* logmsg);

void log_level(severity_level level, const char* logmsg);
void log_level(severity_level level, const wchar_t* logmsg);
void log_level(severity_level level, const std::string& logmsg);
void log_level(severity_level level, const std::wstring& logmsg);
void log_level(severity_level level, const  boost::format& fmt);
void log_level(severity_level level, const boost::wformat& fmt);




} //namespace tlog

#endif