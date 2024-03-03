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
    /*logging::add_file_log(
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
    );*/

    logging::add_console_log(
                std::clog,
                keywords::format = &LogFormatter,
                keywords::auto_flush = true
    );

}

void SendLogInfo(const json::value value, const std::string_view endStr)
{
    BOOST_LOG_TRIVIAL(info) << logging::add_value(additional_data, value)
                            << endStr;

}

void LogFormatter(logging::record_view const& rec, logging::formatting_ostream& strm)
{
    json::object logObj;

    // Момент времени приходится вручную конвертировать в строку.
    // Для получения истинного значения атрибута нужно добавить
    // разыменование.
    auto ts = *rec[timestamp];

    logObj[LogStrKey::data] = *rec[additional_data];
    // Выводим само сообщение.
    logObj[LogStrKey::message] = *rec[logging::expressions::smessage];

    json::value value{{LogStrKey::timestamp, to_iso_extended_string(ts)},
                      {LogStrKey::data, *rec[additional_data]},
                      {LogStrKey::message, *rec[logging::expressions::smessage]},
                     };


    strm <<json::serialize(value);

    //auto ts = *rec[timestamp];
    //strm << "{\"timestamp\":\""sv << to_iso_extended_string(ts) << "\","sv;

    //auto json_data = boost::json::serialize(*rec[additional_data]);

    //strm << "\"data\":"sv << json_data << ","sv;
    //strm << "\"message\":\""sv << rec[boost::log::expressions::smessage] << "\"}"sv;

}

void LogRequest(const std::string address, const std::string uri, const std::string method)
{
    json::object logObj;
    logObj[LogStrKey::address] = address;
    logObj[LogStrKey::uri] = uri;
    logObj[LogStrKey::method] = method;

    json::value value{{LogStrKey::address, address},
                      {LogStrKey::uri, uri},
                      {LogStrKey::method, method}
                     };

    SendLogInfo(value, "request received"sv);
}

void LogReponse(const int response_time, const int code, const std::string content_type)
{
    json::object logObj;
    logObj[LogStrKey::response_time] = response_time;
    logObj[LogStrKey::code] = code;
    logObj[LogStrKey::content_type] = content_type;

    json::value value{{LogStrKey::response_time, response_time},
                      {LogStrKey::code, code},
                      {LogStrKey::content_type, content_type}
                     };
    SendLogInfo(value, "response sent"sv);
}

void LogStartServer(const std::string address, const unsigned int port)
{
    json::object logObj;
    logObj[LogStrKey::address] = address;
    logObj[LogStrKey::port] = port;

    json::value value{{LogStrKey::address, address},
                      {LogStrKey::port, port}
                     };
    SendLogInfo(logObj, "Server has started..."sv);
}

void LogStopServer(const int code)
{
    json::object logObj;
    logObj[LogStrKey::code] = code;

    json::value value{{LogStrKey::code, code}
                     };
    SendLogInfo(value, "server exited"sv);

}

void LogFaultStopServer(const int code, const std::string exception)
{
    json::object logObj;
    logObj[LogStrKey::code] = code;
    logObj[LogStrKey::exception] = exception;

    json::value value{{LogStrKey::code, code},
                      {LogStrKey::exception, exception}
                     };
    SendLogInfo(value, "server exited"sv);
}

void LogNetworkError(const int code, const std::string text, const std::string_view where)
{
    json::object logObj;
    logObj[LogStrKey::code] = code;
    logObj[LogStrKey::text] = text;
    logObj[LogStrKey::where] = std::string{where};

    json::value value{{LogStrKey::code, code},
                      {LogStrKey::text, text},
                      {LogStrKey::where, std::string{where}}
                     };
    SendLogInfo(value, "error"sv);
}


}
