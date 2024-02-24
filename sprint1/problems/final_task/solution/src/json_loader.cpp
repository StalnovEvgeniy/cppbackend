#include "json_loader.h"

#include <iostream>
#include <fstream>

#include <boost/json.hpp>

namespace json_loader {

using namespace model;

using namespace std::literals;

std::string toString(boost::json::string str) {
    return {str.begin(), str.end()};
}

void LoadRoads(Map& map, const boost::json::array& roads) {
    for (const auto& roadElement: roads) {
        const auto& roadObj = roadElement.as_object();

        int start_x = roadObj.at("x0").as_int64();
        int start_y = roadObj.at("y0").as_int64();

        if (roadObj.count("x1")) {
            int end_x = roadObj.at("x1").as_int64();
            map.AddRoad(
                Road{
                    Road::HORIZONTAL,
                    Point { start_x, start_y },
                    end_x
                }
            );
        } else {
            int end_y = roadObj.at("y1").as_int64();
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

void LoadBuildings(Map& map, const boost::json::array& buildings) {
    for (const auto& buildingElement: buildings) {
        const auto& buildingObj = buildingElement.as_object();

        int x = buildingObj.at("x").as_int64();
        int y = buildingObj.at("y").as_int64();
        int width = buildingObj.at("w").as_int64();
        int height = buildingObj.at("h").as_int64();

        map.AddBuilding(Building {
            Rectangle {
                Point { x, y },
                Size { width, height }
            }
        });
    }
}

void LoadOffices(Map& map, const boost::json::array& offices) {
    for (const auto& office_element: offices) {
        const auto& office_obj = office_element.as_object();

        auto id = toString(office_obj.at("id").as_string());
        int x = office_obj.at("x").as_int64();
        int y = office_obj.at("y").as_int64();
        int x_offset = office_obj.at("offsetX").as_int64();
        int y_offset = office_obj.at("offsetY").as_int64();

        map.AddOffice(Office {
            Office::Id(id),
            Point { x, y },
            Offset { x_offset, y_offset }
        });
    }
}

Game LoadGame(const std::filesystem::path& json_path) {
    // Загрузить содержимое файла json_path, например, в виде строки
    // Распарсить строку как JSON, используя boost::json::parse
    // Загрузить модель игры из файла
    Game game;

    std::ifstream stream(json_path);
    std::stringstream buffer;
    buffer << stream.rdbuf();

    auto obj = boost::json::parse(buffer.str()).as_object();

    const auto& maps = obj.at("maps").as_array();
    for (const auto& mapElement: maps) {
        const auto& mapObj = mapElement.as_object();

        auto id = toString(mapObj.at("id").as_string());
        auto name = toString(mapObj.at("name").as_string());
        Map map(Map::Id{id}, name);

        LoadRoads(map, mapObj.at("roads").as_array());
        LoadBuildings(map, mapObj.at("buildings").as_array());
        LoadOffices(map, mapObj.at("offices").as_array());

        game.AddMap(std::move(map));
    }

    return game;
}

}  // namespace json_loader
