#ifndef REQUEST_UTILS_H
#define REQUEST_UTILS_H

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <functional>
#include <string>

//#define BOOST_BEAST_USE_STD_STRING_VIEW

namespace http_handler {

namespace beast = boost::beast;
namespace http = beast::http;

using StringRequest = http::request<http::string_body>;

using namespace std::literals;

constexpr static std::string_view Slesh {"/"sv};
constexpr static std::string_view reqApi {"/api/"sv};
constexpr static std::string_view reqApiV1 {"/api/v1"sv};

struct reqApiV1 {
    reqApiV1() = delete;

    constexpr static std::string_view Maps {"/maps"sv};
    constexpr static std::string_view gamePlayers {"/game/players"sv};
    constexpr static std::string_view gamePlayersAction {"/game/players/action"sv};
    constexpr static std::string_view gameJoin {"/game/join"sv};
    constexpr static std::string_view gameSate {"/game/state"sv};
    constexpr static std::string_view gameTick {"/game/tick"};
};

struct jsonCode {
    constexpr static std::string_view mapNotFound{"mapNotFound"sv};
    constexpr static std::string_view invalidMethod{"invalidMethod"sv};
    constexpr static std::string_view invalidArgument{"invalidArgument"sv};
    constexpr static std::string_view invalidToken{"invalidToken"sv};
    constexpr static std::string_view unknownToken{"unknownToken"sv};
    constexpr static std::string_view badRequest{"badRequest"sv};
    //constexpr static std::string_view mapNotFound{"mapNotFound"sv};
    //constexpr static std::string_view mapNotFound{"mapNotFound"sv};
};

struct jsonMessage {
    constexpr static std::string_view mapNotFound{"Map not found"sv};
    constexpr static std::string_view methodNoPOST{"Only POST method is expected"sv};
    constexpr static std::string_view errorParseJoinGame{"Join game request parse error"sv};
    constexpr static std::string_view invalidName{"Invalid name"sv};
    constexpr static std::string_view errorParseAction{"Failed to parse action"sv};
    constexpr static std::string_view errorParseJSON{"Failed to parse tick request JSON"sv};
    constexpr static std::string_view invalidMethod{"Invalid method"sv};
    constexpr static std::string_view errorAuthorization{"Authorization header is missing"sv};
    constexpr static std::string_view errorPlayerToken{"Player token has not been found"sv};
    constexpr static std::string_view badRequest{"Bad request"sv};
    constexpr static std::string_view errorParseTick{"Failed to parse tick request JSON"sv};

};

struct errorJsonStr{
    errorJsonStr() = delete;
    constexpr static std::pair<std::string_view, std::string_view> mapNotFound{jsonCode::mapNotFound, jsonMessage::mapNotFound};
    constexpr static std::pair<std::string_view, std::string_view> methodNoPOST{jsonCode::invalidMethod, jsonMessage::methodNoPOST};
    constexpr static std::pair<std::string_view, std::string_view> argNoJsonGame{jsonCode::invalidArgument, jsonMessage::errorParseJoinGame};
    constexpr static std::pair<std::string_view, std::string_view> argInvalidName{jsonCode::invalidArgument, jsonMessage::invalidName};
    constexpr static std::pair<std::string_view, std::string_view> invalidArgument{jsonCode::invalidArgument, jsonMessage::errorParseAction};
    constexpr static std::pair<std::string_view, std::string_view> invalidArgumentJson{jsonCode::invalidArgument, jsonMessage::errorParseJSON};
    constexpr static std::pair<std::string_view, std::string_view> invalidMethod{jsonCode::invalidMethod, jsonMessage::invalidMethod};
    constexpr static std::pair<std::string_view, std::string_view> invalidToken{jsonCode::invalidToken, jsonMessage::errorAuthorization};
    constexpr static std::pair<std::string_view, std::string_view> unknownToken{jsonCode::unknownToken, jsonMessage::errorPlayerToken};
    constexpr static std::pair<std::string_view, std::string_view> badRequest{jsonCode::badRequest, jsonMessage::badRequest};
};

struct JsonKey {
    JsonKey() = delete;
    constexpr static auto userName{"userName"};
    constexpr static auto mapId{"mapId"};
    constexpr static auto authToken{"authToken"};
    constexpr static auto playerId{"playerId"};
    constexpr static auto name{"name"};
};

const static std::string strBearer{"Bearer "s};
const static std::string  maskaHexData{"0123456789abcdef"s};

}

#endif // REQUEST_UTILS_H
