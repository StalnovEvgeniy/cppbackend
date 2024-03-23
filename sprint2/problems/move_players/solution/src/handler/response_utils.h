#ifndef RESPONSE_UTILS_H
#define RESPONSE_UTILS_H

#pragma once


//#define BOOST_BEAST_USE_STD_STRING_VIEW

//#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <unordered_map>
#include <filesystem>
#include <string_view>

namespace http_handler {

namespace beast = boost::beast;
namespace http = beast::http;
namespace sys = boost::system;

using namespace std::literals;
namespace fs = std::filesystem;

struct CacheControl {
  CacheControl() = delete;
  constexpr static std::string_view NO_CACHE = "no-cache"sv;
};

struct Allow {
  Allow() = delete;
  constexpr static std::string_view GET_HEAD = "GET, HEAD"sv;
  constexpr static std::string_view POST = "POST"sv;
};

struct ContentType {
    ContentType() = delete;
    constexpr static std::string_view TEXT_HTML = "text/html"sv;
    constexpr static std::string_view TEXT_CSS = "text/css"sv;
    constexpr static std::string_view TEXT_TEXT = "text/plain"sv;
    constexpr static std::string_view TEXT_JS = "text/javascript"sv;
    constexpr static std::string_view APP_JSON = "application/json"sv;
    constexpr static std::string_view APP_XML = "application/xml"sv;
    constexpr static std::string_view IMG_PNG = "image/png"sv;
    constexpr static std::string_view IMG_JPEG = "image/jpeg"sv;
    constexpr static std::string_view IMG_GIF = "image/gif"sv;
    constexpr static std::string_view IMG_BMP = "image/bmp"sv;
    constexpr static std::string_view IMG_ICO = "image/vnd.microsoft.icon"sv;
    constexpr static std::string_view IMG_TIF = "image/tiff"sv;
    constexpr static std::string_view IMG_SVG = "image/svg+xml"sv;
    constexpr static std::string_view AUDIO_MP3 = "audio/mpeg"sv;
    // При необходимости внутрь ContentType можно добавить и другие типы контента
};

struct ExpFile {
    ExpFile() = delete;
    constexpr static std::string_view HTML = ".html"sv;
    constexpr static std::string_view HTM = ".htm"sv;
    constexpr static std::string_view TXT = ".txt"sv;
    constexpr static std::string_view CSS = ".css"sv;
    constexpr static std::string_view JS = ".js"sv;
    constexpr static std::string_view JSON = ".json"sv;
    constexpr static std::string_view XML = ".xml"sv;
    constexpr static std::string_view PNG = ".png"sv;
    constexpr static std::string_view JPG = ".jpg"sv;
    constexpr static std::string_view JPE = ".jpe"sv;
    constexpr static std::string_view JPEG = ".jpeg"sv;
    constexpr static std::string_view GIF = ".gif"sv;
    constexpr static std::string_view BMP = ".bmp"sv;
    constexpr static std::string_view ICO = ".ico"sv;
    constexpr static std::string_view TIFF = ".tiff"sv;
    constexpr static std::string_view TIF = ".tif"sv;
    constexpr static std::string_view SVG = ".svg"sv;
    constexpr static std::string_view SVGZ = ".svgz"sv;
    constexpr static std::string_view MP3 = ".mp3"sv;

};

const static std::unordered_map<std::string_view , std::string_view > mapContextTipe = {
    { ExpFile::HTML, ContentType::TEXT_HTML },
    { ExpFile::HTM,  ContentType::TEXT_HTML },
    { ExpFile::TXT,  ContentType::TEXT_TEXT },
    { ExpFile::CSS,  ContentType::TEXT_CSS },
    { ExpFile::JS,   ContentType::TEXT_JS },
    { ExpFile::JSON, ContentType::APP_JSON },
    { ExpFile::XML,  ContentType::APP_XML },
    { ExpFile::PNG,  ContentType::IMG_PNG },
    { ExpFile::JPG,  ContentType::IMG_JPEG },
    { ExpFile::JPE,  ContentType::IMG_JPEG },
    { ExpFile::JPEG, ContentType::IMG_JPEG },
    { ExpFile::GIF,  ContentType::IMG_GIF },
    { ExpFile::BMP,  ContentType::IMG_BMP },
    { ExpFile::ICO,  ContentType::IMG_ICO },
    { ExpFile::TIFF, ContentType::IMG_TIF },
    { ExpFile::TIF,  ContentType::IMG_TIF },
    { ExpFile::SVG,  ContentType::IMG_SVG },
    { ExpFile::SVGZ, ContentType::IMG_SVG },
    { ExpFile::MP3,  ContentType::AUDIO_MP3 }
};


// Ответ, тело которого представлено в виде строки
using EmptyResponse = http::response<http::string_body>;
using StringResponse = http::response<http::string_body>;
using FileResponse = http::response<http::file_body>;

// Создаёт StringResponse с заданными параметрами
StringResponse MakeStringResponse(http::status status, std::string_view body, unsigned http_version,
                                  bool keep_alive,
                                  std::string_view content_type = ContentType::TEXT_HTML);
StringResponse MakeStringResponse(http::status status, std::string_view body,
                                  unsigned http_version, bool keep_alive,
                                  std::string_view content_type,
                                  std::string_view cache_contro);
StringResponse MakeStringResponse(http::status status, std::string_view body,
                                  unsigned http_version, bool keep_alive,
                                  std::string_view content_type,
                                  std::string_view cache_control,
                                  std::string_view allow);

std::string_view GetMimeType(const std::string_view extension);

FileResponse MakeFileResponse(http::status status, std::string_view filePath, unsigned http_version, bool,
                              std::string_view contentType, sys::error_code& ec);

}


#endif // RESPONSE_UTILS_H
