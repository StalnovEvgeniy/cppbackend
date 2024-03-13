#include "handler.h"
#include "request/utils.h"

#include "path_utils.h"
#include "response/utils.h"
#include "json/parsing.h"

#include "app/player.h"

namespace http_handler {

using namespace response_utuls;
using namespace json_parsing;

namespace json = boost::json;

using namespace std::literals;

/*auto json_response = [&req](http::status status, json::value value) {
    return MakeStringResponse(status, json::serialize(value),
                              req.version(), req.keep_alive(), ContentType::APP_JSON);
};*/

StringResponse RequestHandler::ReportServerError(unsigned http_version, bool keep_alive)
{
    std::string jsonString = json::serialize(json_parsing::GetJsonDataError("badRequest"s, "Bad request"s));
    return MakeStringResponse(http::status::bad_request, jsonString,
                              http_version, keep_alive,
                              ContentType::APP_JSON);
}

RequestHandler::FileRequestResult RequestHandler::HandleFileRequest(const StringRequest &req)
{
    FileRequestResult result{};

    auto text_response = [&req](http::status status, std::string_view text, std::string_view contentType = ContentType::TEXT_HTML) {
        return MakeStringResponse(status, text, req.version(), req.keep_alive(), contentType);
    };


    auto file_response = [&req](http::status status, std::string_view filePath,
            std::string_view contentType, beast::error_code ec) {
        return MakeFileResponse(status, filePath,
                                req.version(), req.keep_alive(), contentType, ec);
    };

    auto target = req.target();
    {
        target.remove_prefix(1);

        fs::path requestPath{target};
        if (requestPath.empty() ||
                requestPath.c_str() == SLESH) {
            requestPath.append(INDEX_HTML);
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
                result = file_response(http::status::ok,
                                       filePath.c_str(),
                                       contentType, ec);
                /*if (!ec){
                        result = response;
                    }*/
                //http::write(adapter, response);
            } else {
                result = text_response(http::status::not_found,
                                       "File not found",
                                       ContentType::TEXT_TEXT);
            }
        } else {
            result = text_response(http::status::bad_request,
                                   "Invalid path",
                                   ContentType::TEXT_TEXT);
        }
    }
    return result;
}

StringResponse RequestHandler::HandleApiRequest(const StringRequest &req)
{
    /*auto json_response_ = [&req](http::status status, json::value value) {
        return MakeStringResponse(status, json::serialize(value),
                                  req.version(), req.keep_alive(), ContentType::APP_JSON);
    };*/
    auto text_response = [&req](http::status status, std::string_view text, std::string_view contentType = ContentType::TEXT_HTML) {
        return MakeStringResponse(status,
                                  text,
                                  req.version(),
                                  req.keep_alive(),
                                  contentType);
    };

    auto json_response = [&req](http::status status, json::value value,
            std::string_view cache_control = "", std::string_view allow = "") {
        return MakeStringResponse(status,
                                  json::serialize(value),
                                  req.version(),
                                  req.keep_alive(),
                                  ContentType::APP_JSON,
                                  cache_control, allow);
    };

    auto target = req.target();

    if (isMapsListRequest(req)) {
        return json_response(http::status::ok,  GetJsonData(game_.GetMaps()));
    } //else
    if (isMapItemRequest(req)) {
        auto id = target.substr(reqListMaps.size() + 1);
        auto map_id = Map::Id{std::string{id}};
        auto* map_ptr = game_.FindMap(map_id);

        if (!map_ptr)
            return json_response(http::status::not_found,
                                   GetJsonDataError("mapNotFound", "Map not found"));
        else
            return json_response(http::status::ok, GetJsonData(*map_ptr));
    }
    if (isApiGameJoinRequest(req)) {
        if (req.method() != http::verb::post){
            return json_response(http::status::method_not_allowed,
                                 GetJsonDataError("invalidMethod", "Only POST method is expected"),
                                 CacheControl::NO_CACHE,
                                 "POST"sv);
        }

        std::string userName;
        std::string mapId;

        try {
          json::value jsonValue = json::parse(req.body());
          userName = jsonValue.as_object().at("userName").as_string();
          mapId = jsonValue.as_object().at("mapId").as_string();
        } catch (...) {
            return json_response(http::status::bad_request,
                                 GetJsonDataError("invalidArgument", "Join game request parse error"),
                                 CacheControl::NO_CACHE);
        }

        if (userName.empty()) {
            return json_response(http::status::bad_request,
                                 GetJsonDataError("invalidArgument", "Invalid name"),
                                 CacheControl::NO_CACHE);
        }

        if (auto map = game_.FindMap(model::Map::Id(mapId)); map == nullptr) {
            return json_response(http::status::not_found,
                                 GetJsonDataError("mapNotFound", "Map not found"),
                                 CacheControl::NO_CACHE);
        }

        json::object jsonObj;
        try {
            auto player = std::make_shared<app::Player> (userName);
            app::Token token = playerTokens_.AddPlayer(player);


          jsonObj = {
              {"authToken", *token},
              {"playerId", *player->GetId()}};

        } catch (const std::exception& e) {
          return text_response(http::status::internal_server_error,
                               "Join Game Error :( call the fixies "s + e.what(),
                               ContentType::TEXT_HTML);
        }
        return json_response(http::status::ok,
                             jsonObj,
                             CacheControl::NO_CACHE);

    }

    if (isApiGamePlayerRequest(req)){
        if (!(req.method() == http::verb::get ||
              req.method() == http::verb::head)) {
            return json_response(http::status::method_not_allowed,
                                 GetJsonDataError("invalidMethod", "Invalid method"),
                                 CacheControl::NO_CACHE,
                                 "GET, HEAD"sv);
        }

        std::string author;

        if (req.count(http::field::authorization)) {
            author = req.at(http::field::authorization);
        } else {
            return json_response(http::status::unauthorized,
                                 GetJsonDataError("invalidToken", "Authorization header is missing"),
                                 CacheControl::NO_CACHE);
        }

        const std::string strCode = "Bearer ";
        if (author.starts_with(strCode)) {
          author = author.substr(strCode.size());
        } else {
            return json_response(http::status::unauthorized,
                                 GetJsonDataError("invalidToken", "Authorization header is missing"),
                                 CacheControl::NO_CACHE);
        }

        std::transform(author.begin(), author.end(), author.begin(), [](unsigned char c) { return std::tolower(c); });

        if (author.find_first_not_of("0123456789abcdef") != std::string::npos) {
            return json_response(http::status::unauthorized,
                                 GetJsonDataError("invalidToken", "Authorization header is missing"),
                                 CacheControl::NO_CACHE);
        }

        if (author.size() != 32) {
            return json_response(http::status::unauthorized,
                                 GetJsonDataError("invalidToken", "Authorization header is missing"),
                                 CacheControl::NO_CACHE);
        }

        app::Token token{author};

        std::shared_ptr<app::Player> player = playerTokens_.FindPlayerByToken(token);
        if (player == nullptr) {
            return json_response(http::status::unauthorized,
                                 GetJsonDataError("unknownToken", "Player token has not been found"),
                                 CacheControl::NO_CACHE);
        }

        // const model::GameSession& gses = p->GetSession();

        json::object jsonObj;

        for (auto it = playerTokens_.GetPlayers().begin(); it != playerTokens_.GetPlayers().end(); ++it) {
         // if (&(it->GetSession()) == &(p->GetSession()))
             std::shared_ptr<app::Player> player = *it;
            jsonObj[std::to_string(*player->GetId())] = boost::json::object{{"name", player->GetName()}};
        }

        return json_response(http::status::ok,
                             jsonObj,
                             CacheControl::NO_CACHE);
        //text_response(http::status::ok, boost::json::serialize(jsonObj), ContentType::APP_JSON, CacheControl::NO_CACHE);
      }


        return json_response(http::status::bad_request,
                               GetJsonDataError("badRequest", "Bad request"));

}



}  // namespace http_handler
