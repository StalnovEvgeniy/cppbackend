#ifndef RANDOMPOINTDOG_H
#define RANDOMPOINTDOG_H

#include <random>

#include "map.h"
#include "types.h"

namespace model {


class RandomPointDog
{
public:
    explicit RandomPointDog(const Map *map);
    RandomPointDog(const RandomPointDog&) = delete;

    //using TokenToPlayer = std::unordered_map<Token, std::shared_ptr<Player>, TokenHasher >;

    PosDouble operator() (){
        if (roads_.empty())
            return PosDouble(0., 0.);

        std::random_device randDev;
        std::mt19937_64 gen1{randDev()};
        int numRoads = 0;
        if (roads_.size() > 1) {
            std::uniform_int_distribution<int> dist(0, roads_.size() - 1);
            numRoads = dist(gen1);
        }

        Road road = roads_.at(numRoads);

        Point start = road.GetStart();
        Point end = road.GetEnd();

        PosDouble pos{static_cast<double>(start.x), static_cast<double>(start.y)};

        double width;
        std::uniform_int_distribution<int> dist(-Road::WidthRoad, Road::WidthRoad);
        width = dist(gen1);

        if (road.IsHorizontal()) {
            std::uniform_int_distribution<int> dist(start.x, end.x);
            pos.x = dist(gen1);
            pos.y += width;
        } else if (road.IsVertical()) {
            std::uniform_int_distribution<int> dist(start.y, end.y);
            pos.x += width;
            pos.y = dist(gen1);
        }

        return pos;
    }

private:
    const Map *map_;
    const Map::Roads &roads_;

};

}

#endif // RANDOMPOINTDOG_H
