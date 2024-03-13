#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#pragma once

#include <boost/json.hpp>

#include "model/game.h"
#include "json/str_literal.h"

namespace json_parsing {

using namespace model;

using namespace json_string;

boost::json::value GetJsonDataError(std::string code, std::string message);
boost::json::value GetJsonData(const Game::Maps& maps);
boost::json::value GetJsonData(const Map& map);

}  // namespace json_serializer


#endif // JSON_PARSING_H
