#include "serial.h"


Serial::Serial(){
	/**
	 * Set attributes for serial device if a single device is available. 
	 * 
	 * @todo Add serial constructor error handling for __WINDOWS__ machines.
	 **/
	try{
#if __linux__
		configure(PORT);

//#elif __APPLE__
//		std::vector<std::string> ports = list_ports();
//		if(ports.size() == 1){
//			PORT = ports[0].c_str();
//			configure(PORT);
//		}
//		else{
//			std::cout << "[NOTE] More that one serial device found.\n"
//				"User must select a serial device before sending commands." << std::endl;
//		}
#endif
	}
	catch(...) {
		std::cout << "[ERROR] Default serial port not opened.\n"
			"User must configure serial port manually." << std::endl;
	}
}


void Serial::configure(const char * port){
	/**
	 * Set attibutes for input ouput communications between serial device and machine.
	 *
	 * @param PORT Serial port to configure.
	 **/
	if(serial_port != 0){
		close(serial_port);
	}
	
	serial_port = open_port(port); //Open port first

	/* 
	 * The termios structure in the termios.h file contains the 
	 * following fields:
	 * 	c_iflag - Terminal input control
	 * 	c_oflag - System output control
	 * 	c_cflag - Hardware control of terminal (Control modes)
	 * 	c_lflag - Terminal functions
	 * 	c_cc    - Special control characters
	 * */
	struct termios tty;

	// Get tty attributes
	if(tcgetattr(serial_port, &tty) != 0){
		std::cout << "Error" << std::endl;
	}

	// c_cflag - Hardware control modes
	tty.c_cflag &= ~PARENB; // Clear parity bit (disable)
	tty.c_cflag &= ~CSTOPB; // Clear stop field (one bit to stop comms)
	tty.c_cflag |= CS8; // Bits per byte (8-bits)
	tty.c_cflag &= ~CRTSCTS; // Disable hardare flow
	tty.c_cflag |= CREAD | CLOCAL; // Enable READ and ignore ctrl lines

	// c_lflag - Terminal functions
	tty.c_lflag &= ~ICANON; // Disable new line proccessing
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable signal interpretation

	// c_iflag - Disable software control
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable flow control
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); 
	// Disable any automatic processing of received bytes

	// c_oflag - Ouput control modes
	tty.c_oflag = 0; // Disable newline carrgage return 
	tty.c_oflag &= ~(OPOST | ONLCR | OCRNL); // Disable processing of output bytes

	// c_cc - Special control characters
	/*An important point to note is that VTIME means slightly different 
	 * things depending on what VMIN is. When VMIN is 0, VTIME 
	 * specifies a time-out from the start of the read() call. 
	 * But when VMIN is > 0, VTIME specifies the time-out from the 
	 * start of the first received character.
	 */
	 // May need to change VTIME up if bytes are clipped
	 tty.c_cc[VTIME] = vTime; // Wait for 10*100ms 
	 tty.c_cc[VMIN] = vMin;
	 
	/*
	 * See https://www.pjrc.com/teensy/td_uart.html for suitable 
	 * baud rates depending on the Teensy model used
	 * Use B9600 if unsure
	 */
	 cfsetispeed(&tty, B115200); // Set input baud rate
	 cfsetospeed(&tty, B115200); // Set output baud rate
	 
	 // Set tty attributes that we just specified
	if(tcsetattr(serial_port, TCSANOW, &tty) != 0){
		std::cout << "[ERROR]: Unable to set serial port attributes." << std::endl;
	}
	close_port();
}


int Serial::open_port(const char* port){
	/**
	 * Open specific serial port.
	 * 
	 * @param PORT Serial port to open.
	 * @returns A const char* to a serial port or 0 if unable to open device.
	 **/
	if(serial_port == 0){
		serial_port = open(port, (O_RDWR | O_NOCTTY | O_NDELAY));
		if (serial_port){
			std::cout << "[OPEN] Port: " << port << std::endl;
			return serial_port;
		}
		else{
			std::cout << "[ERROR] Unable to open serial port: " << PORT << std::endl;
			return 0;
		}	
	}
	else { 
		std::cout << "[OPEN]: Serial port already open" << std::endl;
		return serial_port; 
	}
}


const char* Serial::get_port(){
	/**
	 * Get name of current serial port.
	 *
	 * @returns PORT Serial port name.
	 **/ 
	return PORT;
}


int Serial::get_baudrate(){
	/**
	 * Get the current baudrate of serial communications.
	 * @returns baudRate Current baudrate
	 **/
	return baudRate;
}


void Serial::set_baudrate(int rate){
	/**
	 * Set the baudrate of serial communications.
	 **/
	baudRate = rate;
}


void Serial::send(const char* msg){
	/**
	 * Send message to microcontroller using preset PORT.
	 *
	 * @param msg Message to be sent.
	 **/ 
	serial_port = open_port(PORT);
	if(serial_port){	
		write(serial_port, msg, strlen(msg));
		std::cout << "[SEND]: " << msg << std::endl;
		close_port();
	}
	else{
		std::cout << "[ERROR]: Unable to send command to serial device" << std::endl;
	}
}


void Serial::send(const char * port, const char* msg){
	/**
	 * Send a message to the microcontroller. 
	 *
	 * @param PORT Serial port to send the message to.
	 * @param msg Message to send to the device. 
	 **/
	serial_port = open_port(PORT);
	if(serial_port){
		const char* testMsg = "LR&I:50";
		ssize_t numBytesSent = write(serial_port, testMsg, sizeof(testMsg));
		std::cout << "[ID]: " << serial_port << std::endl;
		std::cout << "[MESSAGE]: " << testMsg << std::endl;
		std::cout << "Message size: " << sizeof(testMsg) << std::endl;
		if(numBytesSent > 0){	
			std::cout << "[SEND]: " << testMsg << " Number of Bytes: " << numBytesSent << std::endl;
		}
		else{
			std::cout << "[ERROR]: Device found, but there was an error "
				"sending command to serial device." << std::endl;
		}
		close_port();
	}
	else{
		std::cout << "[ERROR]: Unable to send command to serial device" << std::endl;
	}
}

void Serial::recieve(){
	if(serial_port != 0){
		int pos = 0;
		char buffer[MAX_READ_SIZE];
		while(pos < MAX_READ_SIZE){
			read(serial_port, &buffer + pos, 1);
			if(buffer[pos] == '\n'){
				break;
			}
			pos++;
		}
		std::cout << "[RECIEVED]: " << buffer << std::endl;
	}
}

void Serial::close_port(){
	/**
	 * Close current serial port.
	 **/ 
	auto retVal = close(serial_port);
	if(retVal != 0){
		std::cout << "[ERROR] Failed to close serial port." << std::endl;
	}
	else{
		serial_port = 0;
		std::cout << "[CLOSED] Serial Port." << std::endl;
	}
}


std::vector<std::string> Serial::list_ports(){	
	/**
	 * List currently connected serial devices.
	 *
	 * @todo Add a windows function here.
	 * @todo Change linux method to match __APPLE__. 
	 * @returns A list of serial devices that are available.
	 **/ 

	std::vector<std::string> ports;
	std::string path;

#if __APPLE__
	path = "/dev/";
	for(const auto & entry : std::filesystem::directory_iterator(path)){
		std::string tty = "/dev/tty.";
		std::string cu = "/dev/cu.";
		if(std::string(entry.path()).std::string::find(tty) != std::string::npos && 
			std::string(entry.path()).std::string::find("Bluetooth") == std::string::npos){	
			auto prt = open(entry.path().c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
			if(prt){
				std::cout << "[FOUND] Port: " << entry.path() << std::endl;
				ports.push_back(entry.path());
				auto retVal = close(prt);
				if(retVal != 0){
					std::cout << "[ERROR] Failed to close serial port." << std::endl;
				}
			}
		}
	}

#elif __linux__
	// TODO adjust linux env serial port connection to the method used for APPLE
	// i.e. using filesystem (c++ 17 required)
	path = "sys/class/tty";
	DIR* dir;
	struct dirent* ent; 
	if((dir = opendir(path.c_str())) != NULL){
		while((ent = readdir(dir)) != NULL){
			struct stat info;
			std::string search_path = path + '/' + ent->d_name + 
										"/device";
			if(lstat(search_path.c_str(), &info) != 0){
				std::cout << "Device not found: " << search_path 
				<< std::endl;
			}
			else{
				std::cout << "Device found: " << search_path 
				<< std::endl;
				ports.push_back(ent->d_name);
			}
		}
		closedir(dir);
	}
	else{
		std::cout << "Error" << std::endl;
	}
#endif

	if(ports.size() == 0){
		ports.push_back("[ERROR] No Serial devices found. Ensure you have a microcontroller "
				"connected to this device.");
	}

	return ports;
}

