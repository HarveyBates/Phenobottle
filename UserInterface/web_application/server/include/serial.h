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


/**
 * Serial functions for communicating between microcontroller and machine.
 **/ 
class Serial{
	const char* PORT = "/dev/tty.usbmodem81500001";
	int serial_port = 0;
	int MAX_READ_SIZE = 20;
	int baudRate = 115200;
	int vTime = 1;
	int vMin = 0;
public:
	Serial();
	void configure(const char * port);
	
	int open_port(const char* port);
	void close_port();
	
	void send(const char* msg);
	void send(const char * port, const char* msg);
	void recieve();
	
	void set_baudrate(int rate);
	int get_baudrate();
	const char* get_port();
	std::vector<std::string> list_ports();
};



