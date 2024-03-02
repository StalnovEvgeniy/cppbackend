#include "response_utuls.h"

#include "path_utils.h"

#include <iostream>

namespace response_utuls {

using namespace path_utils;

// Создаёт StringResponse с заданными параметрами
StringResponse MakeStringResponse(http::status status, std::string_view body,
                                  unsigned http_version, bool keep_alive,
                                  std::string_view content_type) {
    StringResponse response(status, http_version);
    response.set(http::field::content_type, content_type);
    response.body() = body;
    response.content_length(body.size());
    response.keep_alive(keep_alive);
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
                              unsigned http_version, bool keep_alive,
                              std::string_view contentType, sys::error_code& ec)
{
    FileResponse res(status, http_version);


    http::file_body::value_type file;


    //if (ec; file.open(filePath.data(), beast::file_mode::read, ec), ec)  {
    file.open(filePath.data(), beast::file_mode::read, ec);
    //mapContextTipe.at(extension);
    if (!ec) {
        //res.version(11);  // HTTP/1.1
        //res.result(http::status::ok);


        res.insert(http::field::content_type, contentType);
        res.body() = std::move(file);
        res.prepare_payload();
    } else {
        //std::cout << "Failed to open content file!!! "sv << filePath << std::endl;
    }


    //res.prepare_payload();
    return res;

}

}
