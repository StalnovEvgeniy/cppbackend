#include "response/utils.h"

#include "path_utils.h"

#include <iostream>

namespace response_utuls {

using namespace path_utils;

// Создаёт StringResponse с заданными параметрами
StringResponse MakeStringResponse(http::status status, std::string_view body,
                                  unsigned http_version, bool keep_alive,
                                  std::string_view content_type)  {
    StringResponse response(status, http_version);
    response.set(http::field::content_type, content_type);
    response.body() = body;
    response.content_length(body.size());
    response.keep_alive(keep_alive);
    return response;
}

StringResponse MakeStringResponse(http::status status, std::string_view body,
                                  unsigned http_version, bool keep_alive,
                                  std::string_view content_type,
                                  std::string_view cache_control)  {
    StringResponse response = MakeStringResponse(status, body,
                                                 http_version, keep_alive,
                                                 content_type);
    if (!cache_control.empty())
        response.set(http::field::cache_control, cache_control);
    return response;
}

StringResponse MakeStringResponse(http::status status, std::string_view body,
                                  unsigned http_version, bool keep_alive,
                                  std::string_view content_type,
                                  std::string_view cache_control,
                                  std::string_view allow)  {

    StringResponse response = MakeStringResponse(status, body,
                                                 http_version, keep_alive,
                                                 content_type);

    if (!cache_control.empty())
        response.set(http::field::cache_control, cache_control);
    if (!allow.empty())
        response.set(http::field::allow, allow);

    return response;
}

std::string_view GetMimeType(const std::string_view extension)
{
    if (mapContextTipe.contains(extension))
        return mapContextTipe.at(extension);
    else
        return "application/octet-stream"sv;
}

FileResponse MakeFileResponse(http::status status, std::string_view filePath,
                              unsigned http_version, bool /*keep_alive*/,
                              std::string_view contentType, sys::error_code& ec)
{
    FileResponse res(status, http_version);

    http::file_body::value_type file;

    file.open(filePath.data(), beast::file_mode::read, ec);
    if (!ec) {
        res.insert(http::field::content_type, contentType);
        res.body() = std::move(file);
        res.prepare_payload();
    }

    return res;
}

}
