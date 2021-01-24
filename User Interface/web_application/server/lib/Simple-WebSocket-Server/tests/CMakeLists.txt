if(NOT MSVC)
    add_compile_options(-fno-access-control)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wno-thread-safety)
    endif()

    add_executable(swss_parse_test parse_test.cpp)
    target_link_libraries(swss_parse_test simple-websocket-server)
    add_test(swss_parse_test swss_parse_test)
endif()

add_executable(swss_crypto_test crypto_test.cpp)
target_link_libraries(swss_crypto_test simple-websocket-server)
add_test(swss_crypto_test swss_crypto_test)

add_executable(swss_io_test io_test.cpp)
target_link_libraries(swss_io_test simple-websocket-server)
add_test(swss_io_test swss_io_test)
