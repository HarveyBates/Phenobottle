#pragma once
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <filesystem>

class Serial{
	const char* PORT = "tty/ACM0";
	int serial_port;
	int baudRate = 38400;
	int vTime = 10;
	int vMin = 1;
public:
	Serial();
	int open_port(const char* port_ID);
	void set_attributes(const char * port);
	void send(const char * port, const char* msg);
	void close_port();
	std::vector<std::string> list_ports();
};



