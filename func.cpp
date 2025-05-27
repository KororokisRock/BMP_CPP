#include <iostream>
#include <fstream>
#include <string>
#include "func.h"


std::string getPathToInputFile() {
	std::string path_to_file;
	std::cout << "Enter path to input file: ";
	std::getline(std::cin, path_to_file);

	path_to_file.erase(path_to_file.find_last_not_of(" \t\n\r\v\f") + 1);
	path_to_file.erase(0, path_to_file.find_first_not_of(" \t\n\r\v\f"));

	std::ifstream in(path_to_file);
	while (!in.is_open()) {
		std::cout << "Could not open file. Please enter path again: ";
		std::getline(std::cin, path_to_file);

		path_to_file.erase(path_to_file.find_last_not_of(" \t\n\r\v\f") + 1);
		path_to_file.erase(0, path_to_file.find_first_not_of(" \t\n\r\v\f"));
	}

	return path_to_file;
}


std::string getPathToOutputFile() {
	std::string path_to_file;
	std::cout << "Enter path to output file: ";
	std::getline(std::cin, path_to_file);

	path_to_file.erase(path_to_file.find_last_not_of(" \t\n\r\v\f") + 1);
	path_to_file.erase(0, path_to_file.find_first_not_of(" \t\n\r\v\f"));

	return path_to_file;
}