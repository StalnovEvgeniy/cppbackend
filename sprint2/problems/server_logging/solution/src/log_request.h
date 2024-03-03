#ifndef LOG_REQUEST_H
#define LOG_REQUEST_H

#include "request_handler.h"
#include "logger_utils.h"
#include "logger_str_literal.h"
#include <boost/date_time.hpp>

namespace http_handler {

namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace beast = boost::beast;
namespace http = beast::http;

using clock = std::chrono::system_clock;

struct LogDataRequest{
    LogDataRequest( std::string address, HttpRequest& req):
        address{address},
        uri{req.target()},
        method{req.method_string()} {};
    std::string address;
    std::string uri;
    std::string method;
};

struct LogDataResponse{
    LogDataResponse(const long response_time,  const HttpResponse& res):
        response_time{response_time},
        code{res.result_int()},
        content_type(res[http::field::content_type]) {};

    const long response_time;
    const unsigned int code;
    const std::string content_type;
};

template<class SomeRequestHandler>
class LoggingRequestHandler {
    static void LogRequest(const LogDataRequest & data)
    {
        logger_utils::LogRequest(data.address, data.uri, data.method);
    }

    static void LogResponse(LogDataResponse & data)
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
    //LoggingRequestHandler(const LoggingRequestHandler&) = delete;
    //LoggingRequestHandler &operator=(const LoggingRequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(const tcp::endpoint &endpoint, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send)
    {
        LogDataRequest data{endpoint.address().to_string(), req};
        LogRequest(data);


        decorated_(std::move(req), [s = std::move(send)](auto&& response) {
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


     void SetAddress(const std::string address)
     {
         address_.append(address);
     }
private:
     SomeRequestHandler& decorated_;
     std::string address_;

};


}

#endif // LOG_REQUEST_H
