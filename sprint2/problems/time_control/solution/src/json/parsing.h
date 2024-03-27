#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#pragma once

#include <boost/json.hpp>

#include "model/game.h"
#include "json/str_literal.h"

namespace json_parsing {

namespace json = boost::json;

using namespace model;

using namespace json_string;

//json::value GetJsonDataError(std::pair<std::string_view, std::string_view> jsonMessege);
json::value GetJsonDataError(std::string_view code, std::string_view message);
json::value GetJsonData(const Maps& maps);
json::value GetJsonData(const Map& map);
json::object GetJsonPosDog(std::shared_ptr<Dog> dog);

json::value GetJsonDataDirDog(std::shared_ptr<Dog> dog);

}  // namespace json_serializer


#endif // JSON_PARSING_H
