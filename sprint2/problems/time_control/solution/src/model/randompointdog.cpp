#include "randompointdog.h"

using namespace model;

RandomPointDog::RandomPointDog(const Map *map)
    : map_{std::move(map)}
    , roads_(map->GetRoads())
{

}
