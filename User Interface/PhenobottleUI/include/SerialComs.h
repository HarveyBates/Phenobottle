#pragma once
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <string>

class SerialComs{
public:
	int serial_port;
	int baudRate = 38400;
	int vTime = 10;
	int vMin = 1;
	
	int openSerialPort(const char* port_ID);
	void setSerialAttributes(const char * port);
	void sendComs(const char * port, const char* msg);
	void closePort();
	std::vector<std::string> list_ports();
	SerialComs();
};



