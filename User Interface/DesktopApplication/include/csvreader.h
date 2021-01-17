#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <utility> 
#include <sstream>

void write_csv(std::string directory, std::string filename, std::string columnname, std::vector<float> values);
void read_csv(std::string directory, std::string filename, std::string columnname);
bool check_csv_header(std::string filename);