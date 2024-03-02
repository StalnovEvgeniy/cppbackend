#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

#include <boost/log/trivial.hpp>     // для BOOST_LOG_TRIVIAL
#include <boost/log/core.hpp>        // для logging::core
#include <boost/log/expressions.hpp> // для выражения, задающего фильтр

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>


namespace logger_utils {

namespace logging = boost::log;

void InitLogger();
void LogFormatter(logging::record_view const& rec, logging::formatting_ostream& strm);

void LogRequest(const std::string address, std::string uri, std::string method);
void LogReponse(int response_time, int code, std::string content_type);

void LogStartServer(std::string address, unsigned int port);


void LogStopServer(int code);
void LogFaultStopServer(int code, std::string exception);

void LogNetworkError(int code, std::string text, std::string_view where);

}
#endif // LOGGER_UTILS_H
