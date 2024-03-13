#ifndef REQUEST_UTILS_H
#define REQUEST_UTILS_H

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

//#define BOOST_BEAST_USE_STD_STRING_VIEW

namespace request_utuls {

namespace beast = boost::beast;
namespace http = beast::http;

using namespace std::literals;

const std::string_view reqListMaps = "/api/v1/maps"sv;


template <typename Body, typename Allocator>
bool isMapsListRequest(const http::request<Body, http::basic_fields<Allocator> >& req)
{
    auto target = req.target();

    return target == reqListMaps.data();
}


template <typename Body, typename Allocator>
bool isMapItemRequest(const http::request<Body, http::basic_fields<Allocator> >& req)
{
    auto target = req.target();

    return target.starts_with(reqListMaps) && !target.ends_with("/"sv);
}

template <typename Body, typename Allocator>
bool isApiRequest(const http::request<Body, http::basic_fields<Allocator> >& req)
{
    auto target = req.target();

    return target.starts_with("/api/"sv);
}

template <typename Body, typename Allocator>
bool isNotApiRequest(const http::request<Body, http::basic_fields<Allocator> >& req)
{
    auto target = req.target();

    return !target.starts_with("/api/"sv);

}

template <typename Body, typename Allocator>
bool isApiGameJoinRequest(const http::request<Body, http::basic_fields<Allocator> >& req)
{
    auto target = req.target();

    return target.starts_with("/api/v1/game/join"sv);

}

template <typename Body, typename Allocator>
bool isApiGamePlayerRequest(const http::request<Body, http::basic_fields<Allocator> >& req)
{
    auto target = req.target();
    return target.starts_with("/api/v1/game/players"sv);

}

}

#endif // REQUEST_UTILS_H
