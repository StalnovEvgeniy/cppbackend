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
    bool isMapsListRequest(const std::string_view target);
    bool isMapItemRequest(const std::string_view target);
    bool isApiGameJoinRequest(const std::string_view target);
    bool isApiGamePlayerRequest(const std::string_view target);
    bool isApiGameStateRequest(const std::string_view target);

    std::shared_ptr <app::Application> app_;
};

}

#endif // API_HANDLER_H
