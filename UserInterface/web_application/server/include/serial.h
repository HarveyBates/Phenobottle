#pragma once
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <string>
#include <filesystem>
#include <chrono>
#include <thread>

/**
 * Serial functions for communicating between microcontroller and machine.
 **/ 
class Serial{
#if __APPLE__
	const char* PORT = "/dev/tty.usbmodem81500001";
#elif __linux__
	const char* PORT = "/dev/ttyACM0";
#elif __WINDOWS__
	const char* PORT = "COM1";
#endif
	int serial_port = 0;
	int MAX_READ_SIZE = 1024;
	int baudRate = 115200;
	int vTime = 10;
	int vMin = 1;
public:
	Serial();
	void configure(const char * port);
	
	int open_port(const char* port);
	void close_port();
	
	void send(char* msg);
	void send(const char * port, char* msg);
	std::string recieve();
	
	void set_baudrate(int rate);
	int get_baudrate();
	const char* get_port();
	std::vector<std::string> list_ports();
};



