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

struct reqApiV1 {
    reqApiV1() = delete;

    constexpr static std::string_view Maps {"/api/v1/maps"sv};
    constexpr static std::string_view gamePlayers {"/api/v1/game/players"sv};
    constexpr static std::string_view gameJoin {"/api/v1/game/join"sv};
    constexpr static std::string_view gameSate {"/api/v1/game/state"sv};
};

struct errorJsonStr{
    errorJsonStr() = delete;
    constexpr static std::pair<std::string_view, std::string_view> mapNotFound{"mapNotFound"sv, "Map not found"sv};
    constexpr static std::pair<std::string_view, std::string_view> methodNoPOST{"invalidMethod"sv, "Only POST method is expected"sv};
    constexpr static std::pair<std::string_view, std::string_view> argNoJsonGame{"invalidArgument"sv, "Join game request parse error"sv};
    constexpr static std::pair<std::string_view, std::string_view> argInvalidName{"invalidArgument"sv, "Invalid name"sv};
    constexpr static std::pair<std::string_view, std::string_view> invalidMethod{"invalidMethod"sv, "Invalid method"sv};
    constexpr static std::pair<std::string_view, std::string_view> invalidToken{"invalidToken"sv, "Authorization header is missing"sv};
    constexpr static std::pair<std::string_view, std::string_view> unknownToken{"unknownToken"sv, "Player token has not been found"sv};
    constexpr static std::pair<std::string_view, std::string_view> badRequest{"badRequest"sv, "Bad request"sv};
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
