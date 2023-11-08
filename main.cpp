#include <iostream>
#include <fstream>
#include <vector>

std::string reserved_words[] { "return", "int", "float", ";"};

enum class tokenType {
    return_kw,
    int_value,
    float_value,
    semicolon
};

struct token {
    tokenType type;
    std::string value;
};

std::string read_file(std::string file_name);
std::vector<token> lexicalAnalysis(std::string file_contents);

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

token setToken (std::string type, std::string value, bool is_reserved) {
    
}

std::vector<token> lexicalAnalysis(std::string file_contents) {
    std::vector<token> tokens;
    std::string current_element;
    for ( char c : file_contents ) {
        if ( c == ' ' || c == '\n' ) {
            if ( current_element == "" ) {
                continue;
            }
            else {
                bool is_reserved = false;
                for ( std::string word : reserved_words ) {
                    if ( current_element == word ) {
                        is_reserved = true;
                        break;
                    }
                }

            }
        }
        else if ( c == ';' ) {
            token t;
            t.type = tokenType::semicolon;
            t.value = ";";
            tokens.push_back(t);
        }
        else {
            current_element += c;
        }
        std::cout << c << std::endl;
        
    }
    return tokens;
}