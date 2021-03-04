if(UNIX AND NOT APPLE)
	cmake_minimum_required(VERSION "3.3.1")
elseif(APPLE)
	cmake_minimum_required(VERSION "3.19.2")
endif()

# For automatic setup of cpm-cmake
set(CPM_DOWNLOAD_VERSION 0.28.4)
if(CPM_SOURCE_CACHE)
	set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
	set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
	set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
	message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
	file(DOWNLOAD
		https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
		${CPM_DOWNLOAD_LOCATION}
	)
endif()
include(${CPM_DOWNLOAD_LOCATION})

project("phenobottle")

if(UNIX AND NOT APPLE)
	find_package(Threads REQUIRED)
endif()

find_package(OpenSSL REQUIRED)

if(UNIX AND NOT APPLE)
	CPMAddPackage(
		NAME nlohmann_json
		VERSION 3.9.0
		# Just get the latest (3.9.0) version, and only the include folder as the rep is big
		URL https://github.com/nlohmann/json/releases/download/v3.9.0/include.zip
		URL_HASH SHA256=5b9b819aed31626aefe2eace23498cafafc1691890556cd36d2a8002f6905009
	)
elseif(APPLE)
	find_package(nlohmann_json 3.9.0 REQUIRED)
endif()


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

# TODO check OpenSSL access on WINDOWS  
if(APPLE)
	set(OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR} /usr/local/Cellar/openssl@1.1/1.1.1i/)
	set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib CACHE FILEPATH "" FORCE)
	set(OPENSSL_SSL_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libssl.dylib CACHE FILEPATH "" FORCE)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include /usr/local/Cellar/asio/1.18.1/include)

add_executable(${PROJECT_NAME} ${NAME_SRC} ${NAME_HEADERS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(UNIX AND NOT APPLE)
	# As per cpm-cmake requirements
	if (nlohmann_json_ADDED)
		add_library(nlohmann_json INTERFACE IMPORTED)
		target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR})
	endif()
	target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} OpenSSL::SSL OpenSSL::Crypto)
elseif(APPLE)
	target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBRARIES} nlohmann_json::nlohmann_json)
endif()	

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include /usr/local/Cellar/asio/1.18.1/include ${OPENSSL_INCLUDE_DIR})

