cmake_minimum_required(VERSION "3.19.2")

project("sockettest")

find_package(nlohmann_json 3.2.0 REQUIRED)

set(NAME_SRC
	src/main.cpp
    src/server.cpp
    src/connection.cpp)

set(NAME_HEADERS
	include/main.h
    include/server.h
    include/connection.h)

set(EXECUTABLE_OUTPUT_PATH
	${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include /usr/local/Cellar/asio/1.18.1/include)

add_executable(${PROJECT_NAME} ${NAME_SRC} ${NAME_HEADERS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include /usr/local/Cellar/asio/1.18.1/include)



