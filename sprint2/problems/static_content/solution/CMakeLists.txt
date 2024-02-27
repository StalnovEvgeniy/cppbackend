cmake_minimum_required(VERSION 3.11)

project(game_server CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(game_server
	src/main.cpp
        src/http_server.cpp
        src/http_server.h
        src/sdk.h
        src/model.h
        src/model.cpp
        src/tagged.h
        src/boost_json.cpp
        src/json_str_literal.h
        src/json_loader.h
        src/json_loader.cpp
        src/json_parsing.h
        src/json_parsing.cpp
        src/response_utuls.h
        src/response_utuls.cpp
        src/request_handler.cpp
        src/request_handler.h
        src/request_utils.h
        src/sync_write_os.h
        src/path_utils.h
        src/path_utils.cpp
)
target_link_libraries(game_server PRIVATE Threads::Threads)
add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)
