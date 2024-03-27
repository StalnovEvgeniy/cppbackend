#ifndef LOGGER_LOG_UTILS_H
#define LOGGER_LOG_UTILS_H

#pragma once

#include <boost/log/trivial.hpp>     // для BOOST_LOG_TRIVIAL
#include <boost/log/core.hpp>        // для logging::core
#include <boost/log/expressions.hpp> // для выражения, задающего фильтр

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <boost/json.hpp>

namespace logger_utils {

namespace logging = boost::log;
namespace json = boost::json;


void InitLogger();
void LogFormatter(logging::record_view const& rec, logging::formatting_ostream& strm);

void SendLogInfo(const json::value value, const std::string_view endStr);

void LogRequest(const std::string address, const std::string uri, const std::string method);
void LogReponse(const int response_time, const int code, const std::string content_type);

void LogStartServer(const std::string address, const unsigned int port);


void LogStopServer(const int code);
void LogFaultStopServer(const int code, const std::string exception);

void LogNetworkError(const int code, const std::string text, const std::string_view where);

void LogExit(const std::exception& ex);


}
#endif // LOGGER_LOG_UTILS_H
