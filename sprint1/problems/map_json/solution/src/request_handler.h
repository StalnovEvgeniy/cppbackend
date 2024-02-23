#pragma once

#include "sdk.h"

#include "model.h"
#include <boost/json.hpp>
#include <boost/asio/signal_set.hpp>
#include <string>

#include "json_parsing.h"

#include "http_server.h"

namespace http_handler {

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;

namespace net = boost::asio;
namespace sys = boost::system;
using namespace std::literals;

using namespace model;

// Запрос, тело которого представлено в виде строки
using StringRequest = http::request<http::string_body>;
// Ответ, тело которого представлено в виде строки
using StringResponse = http::response<http::string_body>;

struct ContentType {
    ContentType() = delete;
    constexpr static std::string_view TEXT_HTML = "text/html"sv;
    constexpr static std::string_view APP_JSON = "application/json"sv;
    // При необходимости внутрь ContentType можно добавить и другие типы контента
};

// Создаёт StringResponse с заданными параметрами
StringResponse MakeStringResponse(http::status status, std::string_view body, unsigned http_version,
                                  bool keep_alive,
                                  std::string_view content_type = ContentType::TEXT_HTML);
class RequestHandler {
public:
    explicit RequestHandler(model::Game& game)
        : game_{game} {
    }

    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
        // Обработать запрос request и отправить ответ, используя send

        using namespace json_parsing;

        const auto json_response = [&req](http::status status, json::value value) {
            return MakeStringResponse(status, json::serialize(value),
                                      req.version(), req.keep_alive(), ContentType::APP_JSON);
        };

        /*const std::vector<Map>& maps = game_.GetMaps();
        if (maps.empty()) {
            send(json_response(http::status::not_found,
                               GetJsonDataError("mapNotFound"sv, "Map not found")));
            return;
        }*/
        auto target = req.target();
        std::string_view reqListMaps = "/api/v1/maps"sv;
        if (target == reqListMaps.data()) {
            send(json_response(http::status::ok,  GetJsonData(game_.GetMaps())));
            return;
        }
        if (target.starts_with(reqListMaps) && !target.ends_with("/"sv)) {
            std::string_view id = target.substr(reqListMaps.size() + 1);
            auto map_id = Map::Id{std::string{id}};
            auto* map_ptr = game_.FindMap(map_id);

            if (!map_ptr)// == nullptr)
                send(json_response(http::status::not_found,
                                   GetJsonDataError(id, "Map not found")));
            else
                send(json_response(http::status::ok, GetJsonData(*map_ptr)));
            return;
        }
        if (target.starts_with("/api/"sv)) {
            send(json_response(http::status::bad_request,
                               GetJsonDataError("badRequest"sv, "Bad request")));
            return;
        }
        // Здесь можно обработать запрос и сформировать ответ, но пока всегда отвечаем: Hello
        //return text_response(http::status::ok, "<strong>Hello</strong>"sv);

    }

private:
    model::Game& game_;
};

}  // namespace http_handler
