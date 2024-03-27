#include "api_handler.h"
#include "api_handler.h"
#include "api_handler.h"
#include "request_utils.h"

#include "json/parsing.h"

#include "app/player.h"

using namespace http_handler;


ApiHandler::ApiHandler(std::shared_ptr<Application> app, const StringRequest &req)
    : app_{std::move(app)}
    , req_(req)
{

}

StringResponse http_handler::ApiHandler::HandlerApiRequest(/*const StringRequest &req*/)
{
    auto text_response = [&](
            http::status status,
            std::string_view text,
            std::string_view contentType = ContentType::TEXT_HTML
            ) {
        return MakeStringResponse(status,
                                  text,
                                  req_.version(),
                                  req_.keep_alive(),
                                  contentType);
    };

    auto json_response = [&](
            http::status status,
            json::value value,
            std::string_view cache_control = std::string_view(),
            std::string_view allow = std::string_view()
            ) {
        return MakeStringResponse(status,
                                  json::serialize(value),
                                  req_.version(),
                                  req_.keep_alive(),
                                  ContentType::APP_JSON,
                                  cache_control,
                                  allow);
    };

    auto target = req_.target();

    if (!isStartReqApiV1(target))
        return JsonResponse(http::status::bad_request,
                            GetJsonDataError(jsonCode::badRequest,
                                             jsonMessage::badRequest),
                            CacheControl::NO_CACHE);

    target = target.substr(reqApiV1.size());

    if (isStartMaps(target))
        return SendMapList();

    if (isMapItem(target)) {
        return SendMapItem(target);
    }

    if (isApiGameJoin(target)) {
        return SendGameJoin(target);
    }

    if (isApiGamePlayer(target)){
        return GetPlayers();
    }

    if (isApiGameState(target)) {
        return GetGameState();
    }


    if (isApiGameTick(target)){
        return SetGameTick();
        /*if (req_.method() != http::verb::post){
            return json_response(http::status::bad_request,
                                 GetJsonDataError(jsonCode::invalidMethod,
                                                  jsonMessage::methodNoPOST),
                                 CacheControl::NO_CACHE,
                                 Allow::POST);
        }

        try {
            json::value jsonValue = json::parse(req_.body());
            auto timeDelta = jsonValue.as_object().at(MapStrKey::timeDelta).as_double();
            app_->SetTimeDelta(timeDelta);

        } catch (const std::exception& e) {
            JsonResponse(http::status::bad_request,
                                     GetJsonDataError(jsonCode::invalidArgument,
                                                      jsonMessage::errorParseTick),
                                     CacheControl::NO_CACHE);
        }*/

    }

    return JsonResponse(http::status::bad_request,
                         GetJsonDataError(jsonCode::badRequest,
                                          jsonMessage::badRequest),
                         CacheControl::NO_CACHE);
    /*if (!(req_.method() == http::verb::get ||
          req_.method() == http::verb::head)) {
        return json_response(http::status::method_not_allowed,
                             GetJsonDataError(jsonCode::invalidMethod,
                                              jsonMessage::invalidMethod),
                             CacheControl::NO_CACHE,
                             Allow::GET_HEAD);
    }*/



    //std::string author;
    /*if (req_.count(http::field::authorization)) {
        author = req_.at(http::field::authorization);
    } else {
        return json_response(http::status::unauthorized,
                             GetJsonDataError(jsonCode::invalidToken,
                                              jsonMessage::errorAuthorization),
                             CacheControl::NO_CACHE);
    }

    if (author.starts_with(strBearer)){
        author = author.substr(strBearer.size());
    } else {
        return json_response(http::status::unauthorized,
                             GetJsonDataError(jsonCode::invalidToken,
                                              jsonMessage::errorAuthorization),
                             CacheControl::NO_CACHE);
    }


    std::transform(author.begin(), author.end(), author.begin(), [](unsigned char c) { return std::tolower(c); });

    if (author.find_first_not_of(maskaHexData) != std::string::npos ||
            author.size() != 32) {
        return json_response(http::status::unauthorized,
                             GetJsonDataError(jsonCode::invalidToken,
                                              jsonMessage::errorAuthorization),
                             CacheControl::NO_CACHE);
    }*/


    /*Token token(author);

    auto player = app_.FindByToken(token);

    if (player == nullptr) {
        return json_response(http::status::unauthorized,
                             GetJsonDataError(jsonCode::unknownToken,
                                              jsonMessage::errorPlayerToken),
                             CacheControl::NO_CACHE);
    }*/



    /*if (isApiGamePlayer(target)){

        json::object jsonObj;
        for (auto it = app_.GetTokenToPlayer().cbegin(); it != app_.GetTokenToPlayer().cend(); ++it) {
            auto findPlayer = it->second;
             if (findPlayer->GetGameSession() == player->GetGameSession())
                 jsonObj[std::to_string(*findPlayer->GetId())] = boost::json::object{{MapStrKey::name, findPlayer->GetName()}};
        }

        return json_response(http::status::ok,
                             jsonObj,
                             CacheControl::NO_CACHE);
    }*/

    /*if (isApiGameState(target)) {
        json::object jsonObj;
        for (auto it = app_.GetTokenToPlayer().cbegin(); it != app_.GetTokenToPlayer().cend(); ++it) {
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
    if (isApiGamePlayersAction(target)){
        if (req_.method() != http::verb::post){
            return json_response(http::status::method_not_allowed,
                                 GetJsonDataError(jsonCode::invalidMethod,
                                                  jsonMessage::methodNoPOST),
                                 CacheControl::NO_CACHE,
                                 Allow::POST);
        }

        try {
            json::value jsonValue = json::parse(req_.body());
            auto dir = jsonValue.as_object().at(MapStrKey::timeDelta).as_string();
            auto dog = player->GetDog();
            dog->SetDirection(dir.data());

        } catch (const std::exception& e) {
            return json_response(http::status::bad_request,
                                     GetJsonDataError(jsonCode::invalidMethod,
                                                      jsonMessage::invalidMethod),
                                     CacheControl::NO_CACHE);
        }
        return JsonResponse(http::status::ok,
                                 json::object(),
                                 CacheControl::NO_CACHE);
    }

    */



}

bool ApiHandler::isStartReqApiV1(const std::string_view target)
{
    return target.starts_with(reqApiV1);

}

bool ApiHandler::isStartMaps(const std::string_view target)
{
    return (target == reqApiV1::Maps.data());
}

StringResponse ApiHandler::SendMapList()
{
    json::value JsonVal = GetJsonData(app_->ListMap());
    return JsonResponse(http::status::ok, JsonVal);
}



bool ApiHandler::isMapItem(const std::string_view target)
{
    return (target.starts_with(reqApiV1::Maps) && !target.ends_with(Slesh));
}

StringResponse ApiHandler::SendMapItem(const std::string_view target)
{
    auto id = target.substr(reqApiV1::Maps.size() + 1);
    auto map_id = Map::Id{std::string{id}};
    auto map_ptr = app_->FindMap(map_id);

    if (!map_ptr)
        return JsonResponse(http::status::not_found,
                            GetJsonDataError(jsonCode::mapNotFound,
                                             jsonMessage::mapNotFound));
    else
        return JsonResponse(http::status::ok,
                            GetJsonData(*map_ptr));
}

bool ApiHandler::isApiGameJoin(const std::string_view target)
{
    return target.starts_with(reqApiV1::gameJoin);

}

StringResponse ApiHandler::SendGameJoin(const std::string_view target)
{
    if (req_.method() != http::verb::post){
        return JsonResponse(http::status::method_not_allowed,
                            GetJsonDataError(jsonCode::invalidMethod,
                                             jsonMessage::methodNoPOST),
                            CacheControl::NO_CACHE,
                            Allow::POST);
    }

    std::string userName;
    std::string mapId;

    try {
        json::value jsonValue = json::parse(req_.body());
        userName = jsonValue.as_object().at(JsonKey::userName).as_string();
        mapId = jsonValue.as_object().at(JsonKey::mapId).as_string();
    } catch (...) {
        return JsonResponse(http::status::bad_request,
                            GetJsonDataError(jsonCode::invalidArgument,
                                             jsonMessage::errorParseJoinGame),
                            CacheControl::NO_CACHE);
    }

    if (userName.empty()) {
        return JsonResponse(http::status::bad_request,
                            GetJsonDataError(jsonCode::invalidArgument,
                                             jsonMessage::invalidName),
                            CacheControl::NO_CACHE);
    }

    if (auto map = app_->FindMap(model::Map::Id(mapId)); map == nullptr) {
        return JsonResponse(http::status::not_found,
                            GetJsonDataError(jsonCode::mapNotFound,
                                             jsonMessage::mapNotFound),
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
        return JsonResponse(http::status::internal_server_error,
                            {"Join Game Error!!! :( call the fixies "s + e.what( )},
                            ContentType::TEXT_HTML);
    }

    return JsonResponse(http::status::ok,
                        jsonObj,
                        CacheControl::NO_CACHE);
}

bool ApiHandler::isApiGamePlayer(const std::string_view target)
{
    return target.starts_with(reqApiV1::gamePlayers);

}

StringResponse ApiHandler::GetPlayers()
{
    if (!(req_.method() == http::verb::get ||
          req_.method() == http::verb::head)) {
        return JsonResponse(http::status::method_not_allowed,
                            GetJsonDataError(jsonCode::invalidMethod,
                                             jsonMessage::invalidMethod),
                            CacheControl::NO_CACHE,
                            Allow::GET_HEAD);
    }

    return ExecuteAuthorized([this](std::shared_ptr<Player> player){
    //return ExecuteAuthorized([this](const app::Player &player){
        /* Выполняем действие A, используя токен, и возвращаем ответ */
        json::object jsonObj;
        for (auto it = app_->GetTokenToPlayer().cbegin(); it != app_->GetTokenToPlayer().cend(); ++it) {
            auto findPlayer = it->second;
             if (findPlayer->GetGameSession() == player->GetGameSession())
                 jsonObj[std::to_string(*findPlayer->GetId())] = boost::json::object{{MapStrKey::name, findPlayer->GetName()}};
        }

        return JsonResponse(http::status::ok,
                             jsonObj,
                             CacheControl::NO_CACHE);
    });
}

bool ApiHandler::isApiGamePlayersAction(const std::string_view target)
{
    return target.starts_with(reqApiV1::gamePlayersAction);

}

StringResponse ApiHandler::SetPlayerAction()
{
    if (req_.method() != http::verb::post){
        return JsonResponse(http::status::method_not_allowed,
                            GetJsonDataError(jsonCode::invalidMethod,
                                             jsonMessage::methodNoPOST),
                            CacheControl::NO_CACHE,
                            Allow::POST);
    }

    return ExecuteAuthorized([this](std::shared_ptr<Player> player){
    //return ExecuteAuthorized([this](const app::Player &player){
        // Выполняем действие B, используя токен, и возвращаем ответ
        try {
            json::value jsonValue = json::parse(req_.body());
            auto dir = jsonValue.as_object().at(MapStrKey::timeDelta).as_string();
            auto dog = player->GetDog();
            dog->SetDirection(dir.data());

        } catch (const std::exception& e) {
            return JsonResponse(http::status::bad_request,
                                     GetJsonDataError(jsonCode::invalidMethod,
                                                      jsonMessage::invalidMethod),
                                     CacheControl::NO_CACHE);
        }
        return JsonResponse(http::status::ok,
                                 json::object(),
                                 CacheControl::NO_CACHE);
    });
}

bool ApiHandler::isApiGameState(const std::string_view target)
{
    return target.starts_with(reqApiV1::gameSate);

}

StringResponse ApiHandler::GetGameState() {
    if (!(req_.method() == http::verb::get ||
          req_.method() == http::verb::head)) {
        return JsonResponse(http::status::method_not_allowed,
                            GetJsonDataError(jsonCode::invalidMethod,
                                             jsonMessage::invalidMethod),
                            CacheControl::NO_CACHE,
                            Allow::GET_HEAD);
    }

    return ExecuteAuthorized([this](std::shared_ptr<Player> player){
        // Выполняем действие C, используя токен, и возвращаем ответ

        json::object jsonObj;
        for (auto it = app_->GetTokenToPlayer().cbegin(); it != app_->GetTokenToPlayer().cend(); ++it) {
            auto findPlayer = it->second;
            if (findPlayer->GetGameSession() == player->GetGameSession()) {
                jsonObj[std::to_string(*findPlayer->GetId())] = GetJsonPosDog(player->GetDog());
            }
        }

        json::object jsonPlayer;
        jsonPlayer[MapStrKey::players] = jsonObj;

        return JsonResponse(http::status::ok,
                             jsonPlayer,
                             CacheControl::NO_CACHE);
    });
}

bool ApiHandler::isApiGameTick(const std::string_view target)
{
    return target.starts_with(reqApiV1::gameTick);
}

StringResponse ApiHandler::SetGameTick()
{
    if (req_.method() != http::verb::post){
        return JsonResponse(http::status::bad_request,
                             GetJsonDataError(jsonCode::jsonCode::invalidMethod,
                                              jsonMessage::methodNoPOST),
                             CacheControl::NO_CACHE,
                             Allow::POST);
    }

    try {
        json::value jsonValue = json::parse(req_.body());
        auto timeDelta = jsonValue.as_object().at(MapStrKey::timeDelta).as_double();
        app_->SetTimeDelta(timeDelta);

        return JsonResponse(http::status::ok,
                             json::object(),
                             CacheControl::NO_CACHE);
    } catch (const std::exception& e) {
        return JsonResponse(http::status::bad_request,
                                 GetJsonDataError(jsonCode::invalidArgument,
                                                  jsonMessage::errorParseTick),
                                 CacheControl::NO_CACHE);
    }
}

std::optional<Token> ApiHandler::TryExtractToken()
{
    std::string author;

    if (req_.count(http::field::authorization)) {
        author = req_.at(http::field::authorization);
    } else {
        return std::nullopt;
    }

    if (author.starts_with(strBearer)){
        author = author.substr(strBearer.size());
    } else {
        return std::nullopt;
    }

    std::transform(author.begin(), author.end(), author.begin(), [](unsigned char c) { return std::tolower(c); });

    if (author.find_first_not_of(maskaHexData) != std::string::npos ||
            author.size() != 32) {
        return std::nullopt;
    }


    //Token token(author);

    return app::Token(author);
}








