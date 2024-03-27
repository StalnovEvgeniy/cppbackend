#pragma once

#include "sdk.h"

// boost.beast будет использовать std::string_view вместо boost::string_view
//#define BOOST_BEAST_USE_STD_STRING_VIEW

#include "model/game.h"
#include <boost/json.hpp>
#include <boost/asio/signal_set.hpp>
#include <string>
#include <filesystem>
#include <cassert>
#include <variant>

#include "json/parsing.h"

#include "http/server.h"

#include "logging/log_utils.h"

#include "sync_write_os.h"

#include "path_utils.h"

#include "app/playertokens.h"

#include "handler/request_utils.h"
#include "handler/response_utils.h"
#include "handler/api_handler.h"

#include "app/application.h"

namespace http_handler {

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;

namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace sys = boost::system;

using namespace std::literals;

using namespace model;

namespace fs = std::filesystem;

using namespace json_parsing;
using namespace path_utils;
using namespace logger_utils;

//auto application = std::make_shared<app::Application>(game);

class RequestHandler : public std::enable_shared_from_this<RequestHandler> {
//class RequestHandler {

public:

    using Strand = net::strand<net::io_context::executor_type>;

    explicit RequestHandler(fs::path wwwRootPath, Strand api_strand, model::Game& game)//, std::map<ExpansionFile, ContentType>& mapContex)
    //explicit RequestHandler(fs::path wwwRootPath, Strand api_strand, std::shared_ptr<app::Application> app)
        : wwwRootPath_{std::move(wwwRootPath)}
        , api_strand_{std::move(api_strand)}
        //, game_{game}
        ,app_{std::make_shared<app::Application>(game)}
        //, apiHandler_{app_}
    { }


    RequestHandler(const RequestHandler&) = delete;
    //RequestHandler &operator=(const RequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(tcp::endpoint endpoint, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
        auto version = req.version();
        auto keep_alive = req.keep_alive();

        //}
        //void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {//
        // Обработать запрос request и отправить ответ, используя send

        try {
            if (isApiRequest(req)) {
                auto handle = [self = shared_from_this(), send,
                        req = std::forward<decltype(req)>(req), version, keep_alive] {
                    try {
                        // Этот assert не выстрелит, так как лямбда-функция будет выполняться внутри strand
                        assert(self->api_strand_.running_in_this_thread());
                        ApiHandler apiHandler(self->app_, req);
                        //return send(apiHandler.HandlerApiRequest(req));
                        return send(apiHandler.HandlerApiRequest());
                    } catch (...) {
                        send(self->ReportServerError(version, keep_alive));
                    }
                };
                return net::dispatch(api_strand_, handle);
            }
            // Возвращаем результат обработки запроса к файлу
            return std::visit(
                        [&send](auto&& result) {
                send(std::forward<decltype(result)>(result));
            },
            HandleFileRequest(req));

        } catch (...) {
            send(ReportServerError(version, keep_alive));
        }



        //http::write(adapter, req);

    }

private:
    using FileRequestResult = std::variant</*EmptyResponse,*/ StringResponse, FileResponse>;

    FileRequestResult HandleFileRequest(const StringRequest& req);
    StringResponse ReportServerError(unsigned version, bool keep_alive);

    StringResponse JsonResponse(const StringRequest &req, const http::status status, const json::value value)
    {
        std::string jsonString = json::serialize(value);
        return MakeStringResponse(status, jsonString,
                                  req.version(), req.keep_alive(),
                                  ContentType::APP_JSON);
    }

    bool isApiRequest(const StringRequest &req);

    const fs::path wwwRootPath_;
    Strand api_strand_;

    SyncWriteOStreamAdapter adapter{std::cout};

    std::shared_ptr<app::Application> app_;
    //ApiHandler apiHandler_;
};


}  // namespace http_handler
