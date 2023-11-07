#include <iostream>
#include <fstream>
#include <vector>

std::string read_file(std::string file_name);

int main(int argc, char* argv[]) {
    if ( argc != 2 ) {
        std::cout << "usage: Mellow <name.c>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string file_contents;
    file_contents = read_file(argv[1]);

    std::cout << file_contents << std::endl;
    
}

std::string read_file(std::string file_name) {
    std::ifstream file;
    std::string file_contents;

    file.open(file_name);
    if ( file.is_open() ) {
        std::string line;
        while ( getline(file, line) ) {
            file_contents += line + "\n";
        }
    }
    file.close();

    return file_contents;
}