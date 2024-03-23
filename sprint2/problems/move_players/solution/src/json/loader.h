#pragma once

#include <filesystem>

#include "model/game.h"

namespace json_loader {
namespace fs = std::filesystem;

model::Game LoadGame(const fs::path& json_path);

}  // namespace json_loader
