#include "csvreader.h"

void write_csv(std::string directory, std::string filename, std::string columnname, std::vector<float> values){
	
	std::ofstream myFile(directory + filename + ".csv", std::ios::app); // Create Output file
	std::string line;
	std::ifstream fin;
	
	if(myFile.is_open()){
		if(!check_csv_header(filename)){
			myFile << columnname << "\n"; // Send column name to top of file 
		}
		for(auto i : values){
			myFile << i << "\n";
		}
	}
	
	else{
		throw std::runtime_error("Could not open or append local .csv file");
	}
	
	myFile.close();
}


void read_csv(std::string directory, std::string filename, std::string columnname){
	std::ifstream myFile(directory + filename + ".csv");
	std::string result;
	std::string line;
	std::string header;
	int val;
	if(myFile.is_open()){
		if(myFile.good()){
			std::getline(myFile, line); // Gets single line
			std::stringstream linestream(line); // Reads for csv file (similar to std::cin)
			while(std::getline(linestream, header, ',')){
				std::cout << header << std::endl;
			}
			
			int colID = 0;
			
			while(std::getline(myFile, line)){
				std::stringstream linestream(line);
				while(linestream >> val){
					std::cout << colID << " " << val << std::endl;
					if(linestream.peek() == ','){
						linestream.ignore();
					}
					colID++;
				}
			}
		}
	}
}

bool check_csv_header(std::string filename){
	// Checks to see if header labels for the csv file exist
	std::ifstream myFile("/home/pi/Documents/DataOutput/" + filename + ".csv");
	std::string line;
	std::string header;
	if(myFile.is_open()){
		if(myFile.good()){
			std::getline(myFile, line); // Gets single line
			std::stringstream linestream(line); // Reads for csv file (similar to std::cin)
			while(std::getline(linestream, header, ',')){
				std::cout << "Column Name: ";
				std::cout << header << std::endl;
			}
		}
	}	
	if(header.empty()){
		return false;
	}
	else return true;
}
