#include "logger_utils.h"
#include "logger_str_literal.h"

#include <boost/asio/io_context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>


#include <boost/date_time.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>



namespace logger_utils {

namespace logging = boost::log;
namespace json = boost::json;

using namespace std::literals;

namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

BOOST_LOG_ATTRIBUTE_KEYWORD(additional_data, "AdditionalData", json::value);
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

void InitLogger()
{
    logging::add_common_attributes();
    logging::add_file_log(
    keywords::file_name = "sample_%N.log",
    keywords::format = "[%TimeStamp%]: %Message%",
                keywords::open_mode = std::ios_base::app | std::ios_base::out,
                keywords::rotation_size = 10 * 1024 * 1024,
                // ротируем ежедневно в полдень
                keywords::time_based_rotation = sinks::file::rotation_at_time_point(12, 0, 0)
            );

    logging::add_file_log(
                keywords::file_name = "sample.log",
                keywords::format = &LogFormatter
    );

    logging::add_console_log(
                std::clog,
                keywords::format = "[%TimeStamp%]: %Message%",
                keywords::auto_flush = true
    );

}

void SendLogInfo(json::value custom_data)
{
    BOOST_LOG_TRIVIAL(info) << logging::add_value(additional_data, custom_data)
                            << "server exited"sv;

}

void LogFormatter(logging::record_view const& rec, logging::formatting_ostream& strm)
{
    json::object logObj;

    // Момент времени приходится вручную конвертировать в строку.
    // Для получения истинного значения атрибута нужно добавить
    // разыменование.
    auto ts = *rec[timestamp];
    logObj[LogStrKey::timestamp] = to_iso_extended_string(ts) ;
    // Выводим само сообщение.
    logObj[LogStrKey::message] = *rec[logging::expressions::smessage];

    logObj[LogStrKey::data] = *rec[additional_data];

    json::value value{std::move(logObj)};

    strm <<json::serialize(value);

}

void LogRequest(const std::string address, std::string uri, std::string method)
{
    json::object logObj;
    logObj[LogStrKey::address] = address;
    logObj[LogStrKey::uri] = uri;
    logObj[LogStrKey::method] = method;

    json::value value{std::move(logObj)};
    SendLogInfo(value);
}

void LogReponse(int response_time, int code, std::string content_type)
{
    json::object logObj;
    logObj[LogStrKey::response_time] = response_time;
    logObj[LogStrKey::code] = code;
    logObj[LogStrKey::content_type] = content_type;

    json::value value{std::move(logObj)};
    SendLogInfo(value);
}

void LogStartServer(std::string address, unsigned int port)
{
    json::object logObj;
    logObj[LogStrKey::address] = address;
    logObj[LogStrKey::port] = port;

    json::value value{std::move(logObj)};
    SendLogInfo(value);
}

void LogStopServer(int code)
{
    json::object logObj;
    logObj[LogStrKey::code] = code;

    json::value value{std::move(logObj)};
    SendLogInfo(value);

}

void LogFaultStopServer(int code, std::string exception)
{
    json::object logObj;
    logObj[LogStrKey::code] = code;
    logObj[LogStrKey::exception] = exception;

    json::value value{std::move(logObj)};
    SendLogInfo(value);
}

void LogNetworkError(int code, std::string text, std::string_view where)
{
    json::object logObj;
    logObj[LogStrKey::code] = code;
    logObj[LogStrKey::text] = text;
    logObj[LogStrKey::where] = std::string{where};

    json::value value = {std::move(logObj)};
    SendLogInfo(value);
}


}
