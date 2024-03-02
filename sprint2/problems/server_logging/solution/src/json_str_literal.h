#ifndef JSON_STR_LITERAL_H
#define JSON_STR_LITERAL_H

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

};

}

#endif // JSON_STR_LITERAL_H
