#ifndef API_HANDLER_H
#define API_HANDLER_H

#pragma once

#include <memory>
#include <optional>
#include <functional>

#include "handler/request_utils.h"
#include "handler/response_utils.h"

#include <boost/json.hpp>

#include "model/game.h"

#include "app/application.h"

#include "json/parsing.h"

#include "app/players.h"

namespace http_handler {

namespace json = boost::json;
using namespace json_parsing;

using namespace model;
using namespace app;


class ApiHandler
{
public:
    ApiHandler(std::shared_ptr<Application> app, const StringRequest &req);

    StringResponse HandlerApiRequest(/*const StringRequest &req*/);


private:
    StringResponse TextResponse(
            const http::status status,
            const std::string_view text,
            const std::string_view contentType = ContentType::TEXT_HTML
            ) {
        return MakeStringResponse(status,
                                  text,
                                  req_.version(),
                                  req_.keep_alive(),
                                  contentType);
    };

    auto JsonResponse(
            const http::status status,
            const json::value value,
            const std::string_view cache_control = std::string_view(),
            const std::string_view allow = std::string_view()
            ) {
        return MakeStringResponse(status,
                                  json::serialize(value),
                                  req_.version(),
                                  req_.keep_alive(),
                                  ContentType::APP_JSON,
                                  cache_control,
                                  allow);
    };

    bool isStartReqApiV1(const std::string_view target);

    bool isStartMaps(const std::string_view target);
    StringResponse SendMapList();

    bool isMapItem(const std::string_view target);
    StringResponse SendMapItem(const std::string_view target);

    bool isApiGameJoin(const std::string_view target);
    StringResponse SendGameJoin(const std::string_view target);

    bool isApiGamePlayer(const std::string_view target);
    StringResponse GetPlayers(/*const StringRequest& req*/);

    bool isApiGamePlayersAction(const std::string_view target);
    StringResponse SetPlayerAction();

    bool isApiGameState(const std::string_view target);
    StringResponse GetGameState();

    bool isApiGameTick(const std::string_view target);
    StringResponse SetGameTick();


    std::optional<app::Token > TryExtractToken(/*const StringRequest& req*/);

    //StringResponse ExecuteAuthorized(std::function<StringResponse (app::Token & token)> action)
    //StringResponse ExecuteAuthorized(std::function<StringResponse (app::Player & token)> action)
    //{

    template <typename Fn>
    StringResponse ExecuteAuthorized(Fn&& action) {
        auto token = this->TryExtractToken();
        if (token) {
            auto player = app_->FindByToken(*token);
            if (player == nullptr) {
                return JsonResponse(http::status::unauthorized,
                                    json_parsing::GetJsonDataError(jsonCode::unknownToken,
                                                     jsonMessage::errorPlayerToken),
                                    CacheControl::NO_CACHE);
            }
            return action(player);
        } else {
            return JsonResponse(http::status::unauthorized,
                                GetJsonDataError(jsonCode::invalidToken,
                                                 jsonMessage::errorAuthorization),
                                CacheControl::NO_CACHE);
        }
    }




    std::shared_ptr<app::Application> app_;
    const StringRequest &req_;
};

}

#endif // API_HANDLER_H
