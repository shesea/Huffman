#pragma once

#include <string>
#include <vector>

class CLI {
private:
    struct Args {
        std::string input_file = "", output_file = "";
        int flag = 2;
    };
    Args args_;
public:
    void parse(int argc, char * argv[]);

    std::string get_input() const;
    std::string get_output() const;
    int get_flag() const;
};