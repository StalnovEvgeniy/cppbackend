#include "json/loader.h"

#include <iostream>
#include <fstream>

#include <boost/json.hpp>

#include "json/str_literal.h"

namespace json_loader {

using namespace model;

namespace json = boost::json;

using namespace json_string;


std::string toString(json::string str) {
    return {str.begin(), str.end()};
}

void LoadRoads(Map& map, const json::array& roads) {
    for (const auto& roadElement: roads) {
        const auto& roadObj = roadElement.as_object();

        int start_x = roadObj.at(MapStrKey::x0).as_int64();
        int start_y = roadObj.at(MapStrKey::y0).as_int64();

        if (roadObj.count(MapStrKey::x1)) {
            int end_x = roadObj.at(MapStrKey::x1).as_int64();
            map.AddRoad(
                Road{
                    Road::HORIZONTAL,
                    Point { start_x, start_y },
                    end_x
                }
            );
        } else {
            int end_y = roadObj.at(MapStrKey::y1).as_int64();
            map.AddRoad(
                Road{
                    Road::VERTICAL,
                    Point { start_x, start_y },
                    end_y
                }
            );
        }
    }
}

void LoadBuildings(Map& map, const json::array& buildings) {
    for (const auto& buildingElement: buildings) {
        const auto& buildingObj = buildingElement.as_object();

        int x = buildingObj.at(MapStrKey::x).as_int64();
        int y = buildingObj.at(MapStrKey::y).as_int64();
        int width = buildingObj.at(MapStrKey::w).as_int64();
        int height = buildingObj.at(MapStrKey::h).as_int64();

        map.AddBuilding(Building {
            Rectangle {
                Point { x, y },
                Size { width, height }
            }
        });
    }
}

void LoadOffices(Map& map, const json::array& offices) {
    for (const auto& officeElement: offices) {
        const auto& officeObj = officeElement.as_object();

        auto id = toString(officeObj.at(MapStrKey::id).as_string());
        int x = officeObj.at(MapStrKey::x).as_int64();
        int y = officeObj.at(MapStrKey::y).as_int64();
        int x_offset = officeObj.at(MapStrKey::offsetX).as_int64();
        int y_offset = officeObj.at(MapStrKey::offsetY).as_int64();

        map.AddOffice(Office {
            Office::Id(id),
            Point { x, y },
            Offset { x_offset, y_offset }
        });
    }
}

Game LoadGame(const fs::path& json_path) {
    // Загрузить содержимое файла json_path, например, в виде строки
    // Распарсить строку как JSON, используя boost::json::parse
    // Загрузить модель игры из файла
    Game game;

    using namespace std::literals;

    //name_check(json_path);
    //assert(json_path.string == "*.json");
    std::ifstream stream(json_path);
    std::stringstream buffer;
    buffer << stream.rdbuf();

    auto obj = json::parse(buffer.str()).as_object();

    double defDogSpeed{1.0};
    if (obj.contains(MapStrKey::defaultDogSpeed)) {
        defDogSpeed = obj.at(MapStrKey::defaultDogSpeed).as_double();
    }

    const auto& maps = obj.at(MapStrKey::maps).as_array();
    for (const auto& mapElement: maps) {
        const auto& mapObj = mapElement.as_object();

        auto id = toString(mapObj.at(MapStrKey::id).as_string());
        auto name = toString(mapObj.at(MapStrKey::name).as_string());

        double dogSpeed{defDogSpeed};
        if (obj.contains(MapStrKey::dogSpeed)) {
            dogSpeed = obj.at(MapStrKey::dogSpeed).as_double();
        }

        Map map(Map::Id{id}, name, dogSpeed);

        LoadRoads(map, mapObj.at(MapStrKey::roads).as_array());
        LoadBuildings(map, mapObj.at(MapStrKey::buildings).as_array());
        LoadOffices(map, mapObj.at(MapStrKey::offices).as_array());

        game.AddMap(std::move(map));
    }

    return game;
}

}  // namespace json_loader
