#ifndef JSON_STR_LITERAL_H
#define JSON_STR_LITERAL_H

#pragma once

namespace json_string {

struct MapStrKey {
    MapStrKey() = delete;
    constexpr static auto id{"id"};
    constexpr static auto maps{"maps"};
    constexpr static auto name{"name"};
    constexpr static auto roads{"roads"};
    constexpr static auto buildings{"buildings"};
    constexpr static auto offices{"offices"};
    constexpr static auto x0{"x0"};
    constexpr static auto x1{"x1"};
    constexpr static auto y0{"y0"};
    constexpr static auto y1{"y1"};
    constexpr static auto x{"x"};
    constexpr static auto y{"y"};
    constexpr static auto w{"w"};
    constexpr static auto h{"h"};
    constexpr static auto offsetX{"offsetX"};
    constexpr static auto offsetY{"offsetY"};
    constexpr static auto code{"code"};
    constexpr static auto message{"message"};
    constexpr static auto players{"players"};
    constexpr static auto pos{"pos"};
    constexpr static auto speed{"speed"};
    constexpr static auto dir{"dir"};
    constexpr static auto move{"move"};
    constexpr static auto defaultDogSpeed{"defaultDogSpeed"};
    constexpr static auto dogSpeed{"dogSpeed"};

};

}

#endif // JSON_STR_LITERAL_H
