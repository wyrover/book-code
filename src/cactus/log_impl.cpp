
#include "cactus/log_impl.h"

#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
//#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>

#include <boost/core/null_deleter.hpp>
#include <boost/log/utility/setup/file.hpp>					//logging::add_file_log
#include <boost/log/utility/setup/common_attributes.hpp>
// #include <boost/log/utility/setup/console.hpp>
// 
//#include <boost/log/attributes/timer.hpp>
// #include <boost/log/attributes/named_scope.hpp>
// 
#include "boost/log/sources/severity_feature.hpp"			//BOOST_LOG_SEV
// 
#include <boost/log/sources/record_ostream.hpp>
// 
// 
#include <boost/log/support/date_time.hpp>
//
#include <boost/shared_ptr.hpp>
#include <boost/locale/generator.hpp>

//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/barrier.hpp>
//#include <boost/filesystem.hpp>  
//#include <boost/format.hpp>
// //#include <boost/locale/generator.hpp>
//#include <boost/date_time/posix_time/posix_time_types.hpp>

//#include <boost/ref.hpp>
//#include <boost/bind.hpp>

//#include <boost/log/utility/setup/console.hpp>
//#include <boost/log/utility/setup/common_attributes.hpp>

namespace tlog{



namespace logging	= boost::log;
namespace sinks		= boost::log::sinks;
namespace src		= boost::log::sources;
namespace keywords	= boost::log::keywords;
namespace expr		= boost::log::expressions;
namespace attrs		= boost::log::attributes;

BOOST_LOG_ATTRIBUTE_KEYWORD(kw_line_id,		"LineID",	unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(kw_severity,	"Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(kw_timestamp,	"TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(kw_uptime,		"Uptime",	attrs::timer::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(kw_scope,		"Scope",	attrs::named_scope::value_type)

template<>
const char * ChooseCW<char>(const char * c, const wchar_t * w){
	return c;
}

template<>
const wchar_t * ChooseCW<wchar_t>(const char * c, const wchar_t * w){
	return w;
}


void log_msg(const char* logmsg)
{
	my_logger_t& log = my_slog::get();
	BOOST_LOG(log) << logmsg;
	
}

void log_msg(const wchar_t* logmsg)
{
	my_wlogger_t& wlog = my_wlog::get();
	BOOST_LOG(wlog) << logmsg;
}

void log_enter()
{
	my_logger_t& log = my_slog::get();
	BOOST_LOG(log) << __FUNCTION__ << L"(" << __LINE__ << L") <ENTER>";
}

void log_leave()
{
	my_logger_t& log = my_slog::get();
	BOOST_LOG(log) << __FUNCTION__ << L"(" << __LINE__ << L") <LEAVE>";
}


bool log_enable(boost::tribool tbval /*= boost::indeterminate*/)
{
	static bool enable_ = false;

	bool bret = enable_;
	if ( !boost::indeterminate(tbval)){
		enable_ = tbval;
	}

	return bret;
}


int init_stream_log()
{
	if( !log_enable()) return -1;
	return 0;
	try{
		typedef sinks::synchronous_sink<sinks::text_ostream_backend> sink_t;
		typedef sinks::text_ostream_backend backend_t;

		//boost::shared_ptr< std::wostream > strm(&std::wcout, boost::null_deleter()));
		//boost::shared_ptr< std::ostream > strm = boost::make_shared<std::ofstream>("sample.log");
		boost::shared_ptr< std::ostream > strm(new std::ofstream("dghelper.log"));
		strm->imbue(std::locale(LOG_LOCALE, LC_ALL));
		if (!strm->good())
			throw std::runtime_error("Failed to open a text log file");

		// Create a text file sink
		boost::shared_ptr< sink_t > sink = boost::make_shared<sink_t>();
		sink->locked_backend()->add_stream(strm);

		logging::formatter formatter =
			expr::stream
			<< std::setw(6) << std::setfill('0') << kw_line_id << std::setfill(' ') << ": "
			<< "[" << expr::format_date_time(kw_timestamp, "%Y-%m-%d, %H:%M:%S.%f") << "] "
			<< "(" << expr::attr<attrs::current_thread_id::value_type >("ThreadID") << ") "
			<< "<" << kw_severity.or_default(normal) << "> " 
			<< expr::if_(expr::has_attr(kw_scope))
			[
				expr::stream<<"["<<expr::format_named_scope(kw_scope,keywords::format = "%n")<<"]"
			]
			<< ": " << expr::message;

		sink->set_formatter (formatter);

		sink->locked_backend()->auto_flush(true);
		sink->set_filter(expr::attr<severity_level>("Severity") >= normal);   //日志级别过滤
		std::locale loc = boost::locale::generator()("en_US.UTF-8");		//zh_CN.UTF-8	?
		sink->imbue(loc);

		boost::shared_ptr< logging::core > core = logging::core::get();
		core->add_sink(sink);

		logging::add_common_attributes();

		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << "FAILURE: " << e.what() << std::endl;
		return 1;
	}
}



int init_file_log()
{
	if( !log_enable()) return -1;
	return 0;
	try
	{
		// Create a text file sink
		typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;

		boost::shared_ptr< file_sink > sink(new file_sink(
			/*keywords::file_name = "%Y%m%d_%H%M%S_%5N.log",*/
			keywords::file_name = "dghelper.log",
			keywords::open_mode = (std::ios::out | std::ios::trunc),
			keywords::rotation_size = MAX_FILE_SIZE
			//keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0)
		));

// 		boost::shared_ptr< sinks::synchronous_sink< sinks::text_file_backend > > sink = logging::add_file_log
// 			(
// 			"sample.log",
// 			keywords::format = expr::stream
// 			<< expr::format_date_time(timestamp, "%Y-%m-%d, %H:%M:%S.%f")
// 			<< " <" << severity.or_default(normal)
// 			<< "> " << expr::message
// 		);

/*		//使用单个日志文件
		sink->locked_backend()->set_file_collector(sinks::file::make_collector(
			keywords::target = log_forder.c_str(),
			keywords::max_size = MAX_LOG_SIZE,
			keywords::min_free_space = MAX_FREE_SPACE
		));*/

		logging::formatter formatter =
			expr::format("%1%: [%2%] (%3%) <%4%>: %5%")
 			% expr::attr< unsigned int >("RecordID")
			% expr::attr< boost::posix_time::ptime >("TimeStamp")
			//% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
			% expr::attr<attrs::current_thread_id::value_type >("ThreadID")
			% expr::attr<severity_level>("Severity")
			% expr::message;		//expr::wmessage; //expr::smessage;

		sink->set_formatter(formatter);

		sink->locked_backend()->scan_for_files();
		sink->locked_backend()->auto_flush(true);
		sink->set_filter(expr::attr<severity_level>("Severity") >= normal);   //日志级别过滤
		std::locale loc = boost::locale::generator()("en_US.UTF-8");		//zh_CN.UTF-8	?
		sink->imbue(loc);

		boost::shared_ptr< logging::core > core = logging::core::get();
		core->add_sink(sink);

		logging::add_common_attributes();  

		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << "FAILURE: " << e.what() << std::endl;
		return 1;
	}
}

void log_test()
{

	std::wstring wstr(L"wstring 中文");
	std::string str("string english");
	src::severity_logger< severity_level > slg;
	BOOST_LOG_SEV(slg, normal) << "chinese";
	BOOST_LOG_SEV(slg, notification) << "english";
	BOOST_LOG_SEV(slg, warning) << str;


	src::wseverity_logger< severity_level > wslg;
	BOOST_LOG_SEV(wslg, warning) << L"中文";
	BOOST_LOG_SEV(wslg, notification) << L"english";
	BOOST_LOG_SEV(wslg, critical) << wstr;

	src::severity_logger_mt<severity_level>& gslg = my_slogger::get();  
	BOOST_LOG_SEV(gslg, normal) << "chinese";
	BOOST_LOG_SEV(gslg, notification) << "english";
	BOOST_LOG_SEV(gslg, critical) << str;

	src::wseverity_logger_mt<severity_level>& gwslg = my_wslogger::get();
	BOOST_LOG_SEV(gwslg, normal) << L"中文";
	BOOST_LOG_SEV(gwslg, notification) << L"english";
	BOOST_LOG_SEV(gwslg, critical) << wstr;

}

void log_level(severity_level level, const char* logmsg)
{
	severity_logger_t& slg = my_slogger::get();
	BOOST_LOG_SEV(slg, level) << logmsg;
}

void log_level(severity_level level, const wchar_t* logmsg)
{
	wseverity_logger_t& wslg = my_wslogger::get();
	BOOST_LOG_SEV(wslg, level) << logmsg;
}

void log_level(severity_level level, const std::string logmsg)
{
	severity_logger_t& slg = my_slogger::get();
	BOOST_LOG_SEV(slg, level) << logmsg;
}

void log_level(severity_level level, const std::wstring logmsg)
{
	wseverity_logger_t& wslg = my_wslogger::get();
	BOOST_LOG_SEV(wslg, level) << logmsg; 
}

void log_level(severity_level level, const boost::format& fmt)
{
	severity_logger_t& slg = my_slogger::get();
	BOOST_LOG_SEV(slg, level) << boost::str(fmt); 
}

void log_level(severity_level level, const boost::wformat& fmt)
{
	wseverity_logger_t& wslg = my_wslogger::get();
	BOOST_LOG_SEV(wslg, level) << boost::str(fmt); 
}

}//namespace tlog