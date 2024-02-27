#include "json_loader.h"

#include <iostream>
#include <fstream>

#include <boost/json.hpp>

#include "json_str_literal.h"

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

        int start_x = roadObj.at(StrLiteral::x0).as_int64();
        int start_y = roadObj.at(StrLiteral::y0).as_int64();

        if (roadObj.count(StrLiteral::x1)) {
            int end_x = roadObj.at(StrLiteral::x1).as_int64();
            map.AddRoad(
                Road{
                    Road::HORIZONTAL,
                    Point { start_x, start_y },
                    end_x
                }
            );
        } else {
            int end_y = roadObj.at(StrLiteral::y1).as_int64();
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

        int x = buildingObj.at(StrLiteral::x).as_int64();
        int y = buildingObj.at(StrLiteral::y).as_int64();
        int width = buildingObj.at(StrLiteral::w).as_int64();
        int height = buildingObj.at(StrLiteral::h).as_int64();

        map.AddBuilding(Building {
            Rectangle {
                Point { x, y },
                Size { width, height }
            }
        });
    }
}

void LoadOffices(Map& map, const json::array& offices) {
    for (const auto& office_element: offices) {
        const auto& office_obj = office_element.as_object();

        auto id = toString(office_obj.at(StrLiteral::id).as_string());
        int x = office_obj.at(StrLiteral::x).as_int64();
        int y = office_obj.at(StrLiteral::y).as_int64();
        int x_offset = office_obj.at(StrLiteral::offsetX).as_int64();
        int y_offset = office_obj.at(StrLiteral::offsetY).as_int64();

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

    const auto& maps = obj.at(StrLiteral::maps).as_array();
    for (const auto& mapElement: maps) {
        const auto& mapObj = mapElement.as_object();

        auto id = toString(mapObj.at(StrLiteral::id).as_string());
        auto name = toString(mapObj.at(StrLiteral::name).as_string());
        Map map(Map::Id{id}, name);

        LoadRoads(map, mapObj.at(StrLiteral::roads).as_array());
        LoadBuildings(map, mapObj.at(StrLiteral::buildings).as_array());
        LoadOffices(map, mapObj.at(StrLiteral::offices).as_array());

        game.AddMap(std::move(map));
    }

    return game;
}

}  // namespace json_loader
