#pragma once

#include <iostream>
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <thread>
#include <chrono>

#include "connection.h"
#include "server.h"
#include "serial.h"
#include "constants.h"
#include "parsejson.h"
