#ifndef TYPES_H
#define TYPES_H

#pragma once

#include <string_view>
#include <string>

#include <unordered_map>

namespace model {

using namespace std::literals;

using Dimension = int;
using Coord = Dimension;

struct Point {
    Coord x, y;
};

struct Size {
    Dimension width, height;
};

struct Rectangle {
    Point position;
    Size size;
};

struct Offset {
    Dimension dx, dy;
};

template <typename TypeValue>
struct Position {
public:
    Position (const TypeValue &X, const TypeValue &Y)
        : x{X}
        , y{Y}
    {}

    TypeValue x;
    TypeValue y;
};

using PosDouble = Position<double>;
using SpeedDouble = Position<double>;

enum Direction {
    NORTH,
    SOUTH,
    WEST,
    EAST
};

struct DirStr {
    DirStr() = delete;
    constexpr static auto Up{"U"};
    constexpr static auto Down{"D"};
    constexpr static auto Left{"L"};
    constexpr static auto Right{"R"};
};

const static std::unordered_map <std::string , const Direction> StrToDir = {
    {{DirStr::Up},    Direction::NORTH},
    {{DirStr::Down},  Direction::SOUTH},
    {{DirStr::Left},  Direction::WEST},
    {{DirStr::Right}, Direction::EAST}
};
const static std::unordered_map <const Direction, const std::string> DirToStr = {
    {Direction::NORTH, {DirStr::Up}},
    {Direction::SOUTH, {DirStr::Down}},
    {Direction::WEST,  {DirStr::Left}},
    {Direction::EAST,  {DirStr::Right}}
};


}
#endif // TYPES_H
