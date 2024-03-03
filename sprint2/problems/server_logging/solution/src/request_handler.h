#pragma once

#include "sdk.h"

// boost.beast будет использовать std::string_view вместо boost::string_view
//#define BOOST_BEAST_USE_STD_STRING_VIEW

#include "model.h"
#include <boost/json.hpp>
#include <boost/asio/signal_set.hpp>
#include <string>
#include <filesystem>

#include "json_parsing.h"

#include "http_server.h"

#include "response_utuls.h"
#include "request_utils.h"
#include "logger_utils.h"

#include "sync_write_os.h"

#include "path_utils.h"

namespace http_handler {

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;

namespace net = boost::asio;
namespace sys = boost::system;
using namespace std::literals;

using namespace model;

namespace fs = std::filesystem;

using namespace response_utuls;

using namespace request_utuls;

using namespace json_parsing;
using namespace path_utils;
using namespace response_utuls;
using namespace request_utuls;
using namespace logger_utils;

using HttpRequest = http::request<http::string_body>;
using HttpResponse = http::response<http::string_body>;

// Запрос, тело которого представлено в виде строки
//using StringRequest = http::request<http::string_body>;

class RequestHandler {
public:
    explicit RequestHandler(model::Game& game, fs::path& wwwRootPath)//, std::map<ExpansionFile, ContentType>& mapContex)
        : game_{game},
          wwwRootPath_{wwwRootPath} {
    }

    RequestHandler(const RequestHandler&) = delete;
    RequestHandler &operator=(const RequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {//
        // Обработать запрос request и отправить ответ, используя send



        const auto text_response = [&req](http::status status, std::string_view text, std::string_view contentType = ContentType::TEXT_HTML) {
            return MakeStringResponse(status, text, req.version(), req.keep_alive(), contentType);
        };

        auto json_response = [&req](http::status status, json::value value) {
            return MakeStringResponse(status, json::serialize(value),
                                      req.version(), req.keep_alive(), ContentType::APP_JSON);
        };

        auto file_response = [&req](http::status status, std::string_view filePath,
                                    std::string_view contentType, beast::error_code ec) {
            return MakeFileResponse(status, filePath,
                                    req.version(), req.keep_alive(), contentType, ec);
        };

        auto target = req.target();


        //LogRequest(address, target, req.method_string)();


        if (isMapsListRequest(req)) {
        //if (isMapsListRequest(req)) {
            send(json_response(http::status::ok,  GetJsonData(game_.GetMaps())));
        } else if (isMapItemRequest(req)) {

            auto id = target.substr(reqListMaps.size() + 1);
            auto map_id = Map::Id{std::string{id}};
            auto* map_ptr = game_.FindMap(map_id);

            if (!map_ptr)
                send(json_response(http::status::not_found,
                                   GetJsonDataError("mapNotFound"sv, "Map not found"sv)));
            else
                send(json_response(http::status::ok, GetJsonData(*map_ptr)));
        } else if (isApiRequest(req)) {
            send(json_response(http::status::bad_request,
                               GetJsonDataError("badRequest"sv, "Bad request"sv)));
        } else {
            target.remove_prefix(1);

            fs::path requestPath{target};
            //auto filePath = GetPath(requestPath, wwwRootPath_);
            if (requestPath.empty() ||
                    requestPath.c_str() == SLESH) {
                requestPath.append(INDEX_HTML);
                //requestPath.append(SLESH);
            } else
            if (requestPath.c_str() == SLESH) {
               requestPath.append(INDEX_HTML);
            }


            auto filePath = fs::weakly_canonical(wwwRootPath_ / requestPath);

            auto extension = filePath.extension().string();
            auto contentType = GetMimeType(extension);
            if (IsSubPath(filePath, wwwRootPath_)) {
                if (fs::exists(filePath)) {
                        beast::error_code ec;
                        //http::write(adapter, filePath);
                        //std::cout << "Reqeast to open content file "sv << filePath << std::endl;
                        auto response = file_response(http::status::ok, filePath.c_str(), contentType, ec);
                        if (!ec){
                            send(response);
                        }
                        //http::write(adapter, response);
                } else {
                    //std::cout << "Failed to open content file "sv << filePath << std::endl;
                    send(text_response(http::status::not_found, "File not found"sv, ContentType::TEXT_TEXT));
                }
            } else {
                send(text_response(http::status::bad_request, "Invalid path"sv, ContentType::TEXT_TEXT));
            }
        }

        //http::write(adapter, req);
        // Здесь можно обработать запрос и сформировать ответ, но пока всегда отвечаем: Hello
        //return text_response(http::status::ok, "<strong>Hello</strong>"sv);

    }

private:
    model::Game& game_;
    fs::path& wwwRootPath_;
    SyncWriteOStreamAdapter adapter{std::cout};

    //FileResponse getFile(std::string_view target);
    //std::map<std::string_view, std::string_view>& mapContex_;
};


}  // namespace http_handler
