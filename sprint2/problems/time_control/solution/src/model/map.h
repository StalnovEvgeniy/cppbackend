#ifndef MODEL_MAP_H
#define MODEL_MAP_H

#pragma once



#include <string>
#include <unordered_map>
#include <vector>

#include "types.h"

#include "util/tagged.h"

namespace model {


class Road {
    struct HorizontalTag {
        HorizontalTag() = default;
    };

    struct VerticalTag {
        VerticalTag() = default;
    };

public:
    constexpr static HorizontalTag HORIZONTAL{};
    constexpr static VerticalTag VERTICAL{};

    constexpr static double WidthRoad = 0.4;

    Road(HorizontalTag, Point start, Coord end_x) noexcept
        : start_{start}
        , end_{end_x, start.y} {
    }

    Road(VerticalTag, Point start, Coord end_y) noexcept
        : start_{start}
        , end_{start.x, end_y} {
    }

    bool IsHorizontal() const noexcept {
        return start_.y == end_.y;
    }

    bool IsVertical() const noexcept {
        return start_.x == end_.x;
    }

    Point GetStart() const noexcept {
        return start_;
    }

    Point GetEnd() const noexcept {
        return end_;
    }

    PosDouble GetMinPos() const noexcept {
        PosDouble start_d{start_.x, start_.y};
        PosDouble end_d{end_.x, end_.y};
        PosDouble min{start_d.x, start_d.y} ;
        if (IsHorizontal()) {
            min.x = std::min(start_d.x, end_d.x);
        }
        if (IsVertical()) {
            min.y = std::min(start_d.y, end_d.y);
        }
        return min -= WidthRoad;
    }
    PosDouble GetMaxPos() const noexcept {
        PosDouble start_d{start_.x, start_.y};
        PosDouble end_d{end_.x, end_.y};
        PosDouble max{start_d.x, start_d.y} ;
        if (IsHorizontal()) {
            max.x = std::max(start_d.x, end_d.x);
        }
        if (IsVertical()) {
            max.y = std::max(start_d.y, end_d.y);
        }
        return max += WidthRoad;
    }
    bool CheckingPos(PosDouble pos) const noexcept {
        PosDouble min{GetMinPos()};
        PosDouble max{GetMaxPos()};
        if (pos.x >= min.x &&
                pos.y >= min.y &&
                pos.x <= max.x &&
                pos.y <= max.y)
            return true;

        return false;
    }

private:
    Point start_;
    Point end_;
};

class Building {
public:
    explicit Building(Rectangle bounds) noexcept
        : bounds_{bounds} {
    }

    const Rectangle& GetBounds() const noexcept {
        return bounds_;
    }

private:
    Rectangle bounds_;
};

class Office {
public:
    using Id = util::Tagged<std::string, Office>;

    Office(Id id, Point position, Offset offset) noexcept
        : id_{std::move(id)}
        , position_{position}
        , offset_{offset} {
    }

    const Id& GetId() const noexcept;

    Point GetPosition() const noexcept {
        return position_;
    }

    Offset GetOffset() const noexcept {
        return offset_;
    }

private:
    Id id_;
    Point position_;
    Offset offset_;
};

class Map {
public:
    using Id = util::Tagged<std::string, Map>;
    using Roads = std::vector<Road>;
    using Buildings = std::vector<Building>;
    using Offices = std::vector<Office>;

    Map(Id id, std::string name, const double dogSpeed = 0.0) noexcept;

    const Id& GetId() const noexcept;

    const std::string& GetName() const noexcept;

    const Buildings& GetBuildings() const noexcept;

    const Roads& GetRoads() const noexcept;

    const Offices& GetOffices() const noexcept;

    void AddRoad(const Road& road);

    void AddBuilding(const Building& building);

    void AddOffice(Office office);

    double GetDogSpeed(void) const;
    void SetDogSpeed(const double dogSpeed);


private:
    using OfficeIdToIndex = std::unordered_map<Office::Id, size_t, util::TaggedHasher<Office::Id>>;

    Id id_;
    std::string name_;
    Roads roads_;
    Buildings buildings_;

    OfficeIdToIndex warehouse_id_to_index_;
    Offices offices_;

    double dogSpeed_;
};

}

#endif // MODEL_MAP_H
