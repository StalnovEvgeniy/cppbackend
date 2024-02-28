#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include <filesystem>
#include <cassert>

//#define BOOST_BEAST_USE_STD_STRING_VIEW

namespace path_utils {

using namespace std::literals;
namespace fs = std::filesystem;

constexpr static std::string_view SLESH = "/"sv;
constexpr static std::string_view INDEX_HTML = "index.html"sv;


// Возвращает true, если каталог p содержится внутри base_path.
bool IsSubPath(fs::path path, fs::path base);

fs::path GetPath(fs::path target, fs::path rootPath);

}

#endif // PATH_UTILS_H
