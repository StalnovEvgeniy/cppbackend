#ifndef LOG_REQUEST_H
#define LOG_REQUEST_H

#pragma once

#include "request/handler.h"
#include "logging/log_utils.h"
#include "logging/str_literal.h"
#include <boost/date_time.hpp>

#include "response/utils.h"

namespace server_logging {

namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace beast = boost::beast;
namespace http = beast::http;

using clock = std::chrono::system_clock;

using namespace http_handler;
using namespace response_utuls;

struct LogDataRequest{
    LogDataRequest( std::string address, StringRequest& req):
        address{address},
        uri{req.target()},
        method{req.method_string()} {};
    const std::string address;
    const std::string uri;
    const std::string method;
};

struct LogDataResponse{
    LogDataResponse(const long response_time, const StringResponse& res):
        response_time{response_time},
        code{res.result_int()},
        content_type(res[http::field::content_type]) {};

    const long response_time;
    const unsigned int code;
    const std::string content_type;
};

template<class SomeRequestHandler>
class LoggingRequestHandler {
    //template <typename Body, typename Allocator>
    //static void LogRequest(tcp::endpoint &endpoint, StringRequest& req)
    static void LogRequest(LogDataRequest& data)
    {
        logger_utils::LogRequest(data.address, data.uri, data.method);
    }

    static void LogResponse(LogDataResponse& data)
    {
        logger_utils::LogReponse(data.response_time, data.code, data.content_type);
    }

    static void LogResponse(int response_time, int code, std::string content_type)
    {
        logger_utils::LogReponse(response_time, code, content_type);
    }

public:
    LoggingRequestHandler(SomeRequestHandler & decorated):
        decorated_{decorated} {}

    template <typename Body, typename Allocator, typename Send>
    void operator()(const tcp::endpoint &&endpoint, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send)
    {
        LogDataRequest data{endpoint.address().to_string(), req};
        LoggingRequestHandler::LogRequest(data);


        decorated_(std::move(endpoint), std::move(req), [s = std::move(send)](auto&& response) {
        clock::time_point startTime = clock::now();

        clock::time_point stopTime = clock::now();
        const auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count();

        const int code_result = response.result_int();
        const std::string content_type = static_cast<std::string>(response.at(http::field::content_type));

        s(response);

        LogResponse(deltaTime, code_result, content_type);
      });

      return;
    }


     /*void SetAddress(const std::string address)
     {
         address_.append(address);
     }*/
private:
     SomeRequestHandler& decorated_;
     //std::string address_;

};


}

#endif // LOG_REQUEST_H
