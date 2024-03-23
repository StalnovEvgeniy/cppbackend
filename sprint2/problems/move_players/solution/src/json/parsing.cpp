#include "json/parsing.h"

#include <iostream>

namespace json_parsing {



using namespace std::literals;
using namespace model;

boost::json::value GetJsonDataError(std::string_view code, std::string_view message)
{
    json::object jsonObject;

    jsonObject[MapStrKey::code] = code.data();
    jsonObject[MapStrKey::message] = message.data();

    return json::value(std::move(jsonObject));
}

json::value GetJsonDataError(std::pair<std::string_view, std::string_view> jsonMessege)
{
    return GetJsonDataError(jsonMessege.first, jsonMessege.second);
}

namespace gameWigets {

json::value GetJsonData(const Road& road)
{
    json::object object;

    const auto& [start_x, start_y] = road.GetStart();
    object[MapStrKey::x0] = start_x;
    object[MapStrKey::y0] = start_y;

    const auto& [end_x, end_y] = road.GetEnd();
    if (road.IsVertical()) {
        object[MapStrKey::y1] = end_y;
    } else {
        object[MapStrKey::x1] = end_x;
    }

    return json::value(std::move(object));
}

json::value GetJsonData(const Building& building)
{
    json::object object;

    const auto& [position, size] = building.GetBounds();
    object[MapStrKey::x] = position.x;
    object[MapStrKey::y] = position.y;
    object[MapStrKey::w] = size.width;
    object[MapStrKey::h] = size.height;

    return json::value(std::move(object));
}

json::value GetJsonData(const Office& office)
{
    json::object object;

    const auto& id = office.GetId();
    const auto& position = office.GetPosition();
                const auto& offset = office.GetOffset();

                object[MapStrKey::id] = *id;
                object[MapStrKey::x] = position.x;
                object[MapStrKey::y] = position.y;
                object[MapStrKey::offsetX] = offset.dx;
                object[MapStrKey::offsetY] = offset.dy;

                return json::value(std::move(object));
            }

}



json::value GetJsonData(const Maps& maps)
{
    json::array mapsArray;
    for (auto& map :maps ) {
        json::object mapObj;
        mapObj[MapStrKey::id] = *map.GetId();
        mapObj[MapStrKey::name] = map.GetName();

        mapsArray.push_back(mapObj);
    }

    return json::value(std::move(mapsArray));
}

json::value GetJsonData(const Map& map)
{
    json::object mapObj;

    mapObj[MapStrKey::id] = *map.GetId();
    mapObj[MapStrKey::name] = map.GetName();

    json::array roadsArray;
    for (const auto& road: map.GetRoads()) {
        roadsArray.push_back(gameWigets::GetJsonData(road));
    }
    mapObj[MapStrKey::roads] = roadsArray;

    json::array buildingsArray;
    for (const auto& building: map.GetBuildings()) {
        buildingsArray.push_back(gameWigets::GetJsonData(building));
    }
    mapObj[MapStrKey::buildings] = buildingsArray;

    json::array officesArray;
    for (const auto& office: map.GetOffices()) {
        officesArray.push_back(gameWigets::GetJsonData(office));
    }
    mapObj[MapStrKey::offices] = officesArray;

    return json::value(std::move(mapObj));
}

json::object GetJsonPosDog(std::shared_ptr<Dog> dog)
{
    json::object dogObj;
    dogObj[MapStrKey::pos] = json::array{dog->GetPosition().x, dog->GetPosition().y};
    dogObj[MapStrKey::speed] = json::array{dog->GetSpeed().x, dog->GetSpeed().y};
    dogObj[MapStrKey::dir] = dog->GetDirection();


    return dogObj;

}


}
