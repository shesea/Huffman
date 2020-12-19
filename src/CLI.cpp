#include "CLI.h"

#include <iostream>

using std::string;

void CLI::parse(int argc, char* argv[]) {
	if (argc != 6)  
		throw std::runtime_error("Invalid arguments");
	for (int i = 1; i < 5; i++) {
		if (string(argv[i]) == "-u")
			args_.flag = 0;
		if (string(argv[i]) == "-c")
			args_.flag = 1;
		if (string(argv[i]) == "--file" || string(argv[i]) == "-f")
			args_.input_file = std::string(argv[++i]);
		if	(string(argv[i]) == "--output" || std::string(argv[i]) == "-o")
			args_.output_file = std::string(argv[++i]);
	}
	if (args_.flag == 2 || args_.input_file == "" || args_.output_file == "") {
        throw std::runtime_error("Invalid arguments");
    }
}

int CLI::get_flag() const {
    return args_.flag;
}

string CLI::get_input() const {
    return args_.input_file;
} 

string CLI::get_output() const {
    return args_.output_file;
}