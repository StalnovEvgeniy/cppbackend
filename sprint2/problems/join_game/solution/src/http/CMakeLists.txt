
include_directories(../)
set(SRC_HTTP http_server.cpp)
set(INC_HTTP http_server.h)

#add_library(libHTTP ${SRC_HTTP} ${INC_HTTP})

target_sources(${PROJECT_NAME} PUBLIC SRC_HTTP)
#target_include_directories(libHTTP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
