#ifndef API_HANDLER_H
#define API_HANDLER_H

#pragma once

#include <memory>
#include <optional>
#include "handler/request_utils.h"
#include "handler/response_utils.h"

#include <boost/json.hpp>

#include "model/game.h"

#include "app/application.h"

#include "app/players.h"

namespace http_handler {

namespace json = boost::json;

class ApiHandler
{
public:
    ApiHandler(std::shared_ptr <app::Application> app);

    StringResponse HandlerApiRequest(const StringRequest &req);


private:
    bool isStartReqApiV1(const std::string_view target);
    bool isStartMaps(const std::string_view target);
    bool isMapItemRequest(const std::string_view target);
    bool isApiGameJoinRequest(const std::string_view target);
    bool isApiGamePlayerRequest(const std::string_view target);
    bool isApiGamePlayersActionRequest(const std::string_view target);
    bool isApiGameStateRequest(const std::string_view target);

    std::optional<app::Token> TryExtractToken(const StringRequest& request);

    template <typename Fn>
    StringResponse ExecuteAuthorized(Fn&& action) {
        /*if (auto token = TryExtractToken(request)) {
            return action(*token);
        } else {
            return MakeUnauthorizedError();
        }*/
    }


    StringResponse GetPlayers(const StringRequest& request);

    StringResponse SetPlayerAction(const StringRequest& request);

    StringResponse GetGameState(const StringRequest& request);

    std::shared_ptr <app::Application> app_;
};

}

#endif // API_HANDLER_H
