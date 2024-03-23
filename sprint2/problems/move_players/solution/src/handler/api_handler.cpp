#include "api_handler.h"
#include "api_handler.h"
#include "request_utils.h"

#include "json/parsing.h"

#include "app/player.h"


using namespace http_handler;
using namespace json_parsing;

using namespace model;
using namespace app;

ApiHandler::ApiHandler(std::shared_ptr<Application> app)
    : app_{std::move(app)}
{

}


StringResponse http_handler::ApiHandler::HandlerApiRequest(const StringRequest &req)
{
    auto text_response = [&req](
            http::status status,
            std::string_view text,
            std::string_view contentType = ContentType::TEXT_HTML
            ) {
        return MakeStringResponse(status,
                                  text,
                                  req.version(),
                                  req.keep_alive(),
                                  contentType);
    };

    auto json_response = [&req](
            http::status status,
            json::value value,
            std::string_view cache_control = std::string_view(),
            std::string_view allow = std::string_view()
            ) {
        return MakeStringResponse(status,
                                  json::serialize(value),
                                  req.version(),
                                  req.keep_alive(),
                                  ContentType::APP_JSON,
                                  cache_control,
                                  allow);
    };

    auto baseTarget = req.target();

    if (!isStartReqApiV1(baseTarget))
        return json_response(http::status::bad_request,
                             GetJsonDataError(errorJsonStr::badRequest),
                             CacheControl::NO_CACHE);

    auto target = baseTarget.substr(reqApiV1.size());

    if (isStartMaps(target))
        return json_response(http::status::ok,
                             GetJsonData(app_->ListMap()));

    if (isMapItemRequest(target)) {
        auto id = target.substr(reqApiV1::Maps.size() + 1);
        auto map_id = Map::Id{std::string{id}};
        auto map_ptr = app_->FindMap(map_id);

        if (!map_ptr)
            return json_response(http::status::not_found,
                                 GetJsonDataError(errorJsonStr::mapNotFound));
        else
            return json_response(http::status::ok,
                                 GetJsonData(*map_ptr));
    }

    if (isApiGameJoinRequest(target)) {
        if (req.method() != http::verb::post){
            return json_response(http::status::method_not_allowed,
                                 GetJsonDataError(errorJsonStr::methodNoPOST),
                                 CacheControl::NO_CACHE,
                                 Allow::POST);
        }

        std::string userName;
        std::string mapId;

        try {
            json::value jsonValue = json::parse(req.body());
            userName = jsonValue.as_object().at(JsonKey::userName).as_string();
            mapId = jsonValue.as_object().at(JsonKey::mapId).as_string();
        } catch (...) {
            return json_response(http::status::bad_request,
                                 GetJsonDataError(errorJsonStr::argNoJsonGame),
                                 CacheControl::NO_CACHE);
        }

        if (userName.empty()) {
            return json_response(http::status::bad_request,
                                 GetJsonDataError(errorJsonStr::argInvalidName),
                                 CacheControl::NO_CACHE);
        }

        if (auto map = app_->FindMap(model::Map::Id(mapId)); map == nullptr) {
            return json_response(http::status::not_found,
                                 GetJsonDataError(errorJsonStr::mapNotFound),
                                 CacheControl::NO_CACHE);
        }

        json::object jsonObj;
        try {
            //auto token
            auto pair_= app_->JoinGame(Map::Id(mapId), userName);

            auto player = pair_.first;
            auto token = pair_.second;

            jsonObj = {
                {JsonKey::authToken, *token},
                {JsonKey::playerId, *player->GetId()}};

        } catch (const std::exception& e) {
            return text_response(http::status::internal_server_error,
                                 "Join Game Error!!! :( call the fixies "s + e.what( ),
                                 ContentType::TEXT_HTML);
        }

        return json_response(http::status::ok,
                             jsonObj,
                             CacheControl::NO_CACHE);

    }

    if (!(req.method() == http::verb::get ||
          req.method() == http::verb::head)) {
        return json_response(http::status::method_not_allowed,
                             GetJsonDataError(errorJsonStr::invalidMethod),
                             CacheControl::NO_CACHE,
                             Allow::GET_HEAD);
    }

    std::string author;

    if (req.count(http::field::authorization)) {
        author = req.at(http::field::authorization);
    } else {
        return json_response(http::status::unauthorized,
                             GetJsonDataError(errorJsonStr::invalidToken),
                             CacheControl::NO_CACHE);
    }

    if (author.starts_with(strBearer)){
        author = author.substr(strBearer.size());
    } else {
        return json_response(http::status::unauthorized,
                             GetJsonDataError(errorJsonStr::invalidToken),
                             CacheControl::NO_CACHE);
    }


    std::transform(author.begin(), author.end(), author.begin(), [](unsigned char c) { return std::tolower(c); });

    if (author.find_first_not_of(maskaHexData) != std::string::npos ||
            author.size() != 32) {
        return json_response(http::status::unauthorized,
                             GetJsonDataError(errorJsonStr::invalidToken),
                             CacheControl::NO_CACHE);
    }


    Token token(author);

    auto player = app_->FindByToken(token);

    if (player == nullptr) {
        return json_response(http::status::unauthorized,
                             GetJsonDataError(errorJsonStr::unknownToken),
                             CacheControl::NO_CACHE);
    }




    if (isApiGamePlayerRequest(target)){
        json::object jsonObj;
        for (auto it = app_->GetTokenToPlayer().cbegin(); it != app_->GetTokenToPlayer().cend(); ++it) {
            auto findPlayer = it->second;
             if (findPlayer->GetGameSession() == player->GetGameSession())
                 jsonObj[std::to_string(*findPlayer->GetId())] = boost::json::object{{MapStrKey::name, findPlayer->GetName()}};
        }

        return json_response(http::status::ok,
                             jsonObj,
                             CacheControl::NO_CACHE);
    }

    if (isApiGameStateRequest(target)) {
        json::object jsonObj;
        for (auto it = app_->GetTokenToPlayer().cbegin(); it != app_->GetTokenToPlayer().cend(); ++it) {
            auto findPlayer = it->second;
            if (findPlayer->GetGameSession() == player->GetGameSession())
            {
                try {
                    std::shared_ptr<Dog> dog = findPlayer->GetDog();
                    jsonObj[std::to_string(*findPlayer->GetId())] = GetJsonPosDog(findPlayer->GetDog());
                } catch (const std::exception& e) {
                    return text_response(http::status::internal_server_error,
                                         "Read Class Dog Error!!! :( call the fixies "s + e.what( ),
                                         ContentType::TEXT_HTML);
                }
            }
        }

        json::object jsonPlayer;
        jsonPlayer[MapStrKey::players] = jsonObj;

        return json_response(http::status::ok,
                             jsonPlayer,
                             CacheControl::NO_CACHE);
    }

    if (isApiGamePlayersActionRequest(target)){
        if (req.method() != http::verb::post){
            return json_response(http::status::method_not_allowed,
                                 GetJsonDataError(errorJsonStr::methodNoPOST),
                                 CacheControl::NO_CACHE,
                                 Allow::POST);
        }

        try {
            json::value jsonValue = json::parse(req.body());
            auto dir = jsonValue.as_object().at(MapStrKey::move).as_string();
            auto dog = player->GetDog();
            dog->SetDirection(dir.data());

        } catch (const std::exception& e) {
            json_response(http::status::bad_request,
                                     GetJsonDataError(errorJsonStr::invalidMethod),
                                     CacheControl::NO_CACHE);
        }
    }

    return json_response(http::status::bad_request,
                         GetJsonDataError(errorJsonStr::badRequest),
                         CacheControl::NO_CACHE);
}

bool ApiHandler::isStartReqApiV1(const std::string_view target)
{
    return target.starts_with(reqApiV1);

}

bool ApiHandler::isStartMaps(const std::string_view target)
{
    //return target.starts_with(reqApiV1::Maps);
    return (target == reqApiV1::Maps.data());
}

bool ApiHandler::isMapItemRequest(const std::string_view target)
{
    return (target.starts_with(reqApiV1::Maps) && !target.ends_with(Slesh));
}

bool ApiHandler::isApiGameJoinRequest(const std::string_view target)
{
    return target.starts_with(reqApiV1::gameJoin);

}

bool ApiHandler::isApiGamePlayerRequest(const std::string_view target)
{
    return target.starts_with(reqApiV1::gamePlayers);

}

bool ApiHandler::isApiGamePlayersActionRequest(const std::string_view target)
{
    return target.starts_with(reqApiV1::gamePlayersAction);

}

bool ApiHandler::isApiGameStateRequest(const std::string_view target)
{
    return target.starts_with(reqApiV1::gameSate);

}


