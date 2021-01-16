cmake_minimum_required(VERSION "3.19.2")

project("PhenobottleUI")

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

set(NAME_SRC 
	src/main.cpp
	src/mainframe.cpp
	src/csvreader.cpp
	src/growlights.cpp
	src/phframe.cpp
	src/SerialComs.cpp
	src/wxchartviewer.cpp)

set(NAME_HEADERS 
	include/main.h
	include/mainframe.h
	include/csvreader.h
	include/phframe.h
	include/SerialComs.h
	include/growlights.h
	include/wxchartviewer_defs.h
	include/wxchartviewer_version.h
	include/wxchartviewer.h
	include/chartdir.h)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(chartdir STATIC IMPORTED)
set_target_properties(chartdir PROPERTIES
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/libchartdir.6.dylib")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${NAME_SRC} ${NAME_HEADERS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} chartdir)
