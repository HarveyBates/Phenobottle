cmake_minimum_required(VERSION "3.19.2")

project("sockettest")

find_package(nlohmann_json 3.2.0 REQUIRED)

set(NAME_SRC
	src/main.cpp
    src/server.cpp
    src/connection.cpp
	src/serial.cpp)

set(NAME_HEADERS
	include/main.h
	include/constants.h
    include/server.h
    include/connection.h
	include/crypto.hpp
	include/serial.h)

set(EXECUTABLE_OUTPUT_PATH
	${CMAKE_BINARY_DIR}/bin)

# TODO check OpenSSL access on linux and windows 
if(APPLE)
	set(OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR} /usr/local/Cellar/openssl@1.1/1.1.1i/)
	set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib CACHE FILEPATH "" FORCE)
	set(OPENSSL_SSL_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libssl.dylib CACHE FILEPATH "" FORCE)
endif()

find_package(OpenSSL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include /usr/local/Cellar/asio/1.18.1/include)

add_executable(${PROJECT_NAME} ${NAME_SRC} ${NAME_HEADERS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json ${OPENSSL_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include /usr/local/Cellar/asio/1.18.1/include ${OPENSSL_INCLUDE_DIR})



