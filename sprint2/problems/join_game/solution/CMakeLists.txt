cmake_minimum_required(VERSION 3.11)

project(game_server CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup(TARGETS)

find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories("src/")

add_executable(game_server
	src/main.cpp
        src/http/server.cpp
        src/http/server.h
        src/sdk.h
        src/model/game.h
        src/model/game.cpp
        src/model/map.h
        src/model/map.cpp
        src/model/gamesession.h
        src/model/gamesession.cpp
        src/model/dog.h
        src/model/dog.cpp
        src/app/player.h
        src/app/player.cpp
        src/app/playertokens.h
        src/app/playertokens.cpp
        src/util/tagged.h
        src/boost_json.cpp
        src/json/str_literal.h
        src/json/loader.h
        src/json/loader.cpp
        src/json/parsing.h
        src/json/parsing.cpp
        src/response/utils.h
        src/response/utils.cpp
        src/request/handler.cpp
        src/request/handler.h
        src/request/utils.h
        src/sync_write_os.h
        src/path_utils.h
        src/path_utils.cpp
        src/logging/log_utils.h
        src/logging/log_utils.cpp
        src/logging/str_literal.h
        src/logging/str_literal.cpp
        src/log_request.h
        src/log_request.cpp
)

#target_link_libraries(${PROJECT_NAME} libHTTP)

# используем "импортированную" цель CONAN_PKG::boost
target_include_directories(${PROJECT_NAME} PRIVATE CONAN_PKG::boost)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads  CONAN_PKG::boost)

add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)
