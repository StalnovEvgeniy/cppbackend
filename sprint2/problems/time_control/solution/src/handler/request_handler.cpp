#include "handler/request_handler.h"
#include "handler/request_utils.h"
#include "handler/response_utils.h"

#include "path_utils.h"
#include "json/parsing.h"

#include "app/player.h"

namespace http_handler {

using namespace json_parsing;

namespace json = boost::json;

using namespace std::literals;

StringResponse RequestHandler::ReportServerError(unsigned http_version, bool keep_alive)
{
    std::string jsonString = json::serialize(json_parsing::GetJsonDataError(jsonCode::badRequest,
                                                                            jsonMessage::badRequest));
    return MakeStringResponse(http::status::bad_request, jsonString,
                              http_version, keep_alive,
                              ContentType::APP_JSON);
}

bool RequestHandler::isApiRequest(const StringRequest &req)
{
    auto target = req.target();
    return target.starts_with(reqApi);
    //return target.starts_with(reqApiV1);
}




RequestHandler::FileRequestResult RequestHandler::HandleFileRequest(const StringRequest &req)
{
    FileRequestResult result{};

    auto text_response = [&req](http::status status, std::string_view text, std::string_view contentType = ContentType::TEXT_HTML) {
        return MakeStringResponse(status, text, req.version(), req.keep_alive(), contentType);
    };


    auto file_response = [&req](http::status status, std::string_view filePath,
            std::string_view contentType, beast::error_code ec) {
        return MakeFileResponse(status, filePath,
                                req.version(), req.keep_alive(), contentType, ec);
    };

    auto target = req.target();
    {
        target.remove_prefix(1);

        fs::path requestPath{target};
        if (requestPath.empty() ||
                requestPath.c_str() == SLESH) {
            requestPath.append(INDEX_HTML);
        } else
            if (requestPath.c_str() == SLESH) {
                requestPath.append(INDEX_HTML);
            }


        auto filePath = fs::weakly_canonical(wwwRootPath_ / requestPath);

        auto extension = filePath.extension().string();
        auto contentType = GetMimeType(extension);
        if (IsSubPath(filePath, wwwRootPath_)) {
            if (fs::exists(filePath)) {
                beast::error_code ec;
                result = file_response(http::status::ok,
                                       filePath.c_str(),
                                       contentType, ec);
                /*if (!ec){
                        result = response;
                    }*/
                //http::write(adapter, response);
            } else {
                result = text_response(http::status::not_found,
                                       "File not found",
                                       ContentType::TEXT_TEXT);
            }
        } else {
            result = text_response(http::status::bad_request,
                                   "Invalid path",
                                   ContentType::TEXT_TEXT);
        }
    }
    return result;
}

}  // namespace http_handler
