#include <iostream>
#include <fstream>
#include <vector>

std::string reserved_words[] { "return", "int", "float", ";"};
char operators[] { '=', '+', '-', '*' };


enum class tokenType {
    return_kw,
    int_type,
    float_type,
    int_value,
    float_value,
    identifier,

    equal_operator,
    addition_operator,
    subtraction_operator,
    multiply_operator,
    semicolon,
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
    std::vector<token> tokens = lexicalAnalysis(file_contents);

    for ( token t : tokens ) {
        std::cout << t.value << std::endl;
    }
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

token setToken (std::string value, bool is_reserved, bool is_operator, bool is_digit) {
    token newToken;
    if (is_reserved) {
        if (value == "int") {
            newToken.type = tokenType::int_type;
            newToken.value = value;
        }
        else if (value == "float") {  
            newToken.type = tokenType::float_type;
            newToken.value = value;
        }
        else if (value == "return") {           
            newToken.type = tokenType::return_kw;
            newToken.value = value;
        }
    }
    else if (is_operator) {
        if (value == "=") {
            newToken.type = tokenType::equal_operator;
            newToken.value = value;
        }
        else if (value == "+") {
            newToken.type = tokenType::addition_operator;
            newToken.value = value;
        }
        else if (value == "-") {
            newToken.type = tokenType::subtraction_operator;
            newToken.value = value;
        }
        else if (value == "*") {
            newToken.type = tokenType::multiply_operator;
            newToken.value = value;
        
        }
    }
    else if(is_digit) {
        if (value.find(".") != std::string::npos) {
            newToken.type = tokenType::float_value;
            newToken.value = value;
        }
        else {
            newToken.type = tokenType::int_value;
            newToken.value = value;
        }
    }
    else {
            newToken.type = tokenType::identifier;
            newToken.value = value;
        }

    return newToken;
}

std::vector<token> lexicalAnalysis(std::string file_contents) {
    std::vector<token> tokens;
    std::string current_element;
    for ( char c : file_contents ) {
        bool is_operator = false;
        for ( char operator_char : operators ) {
            if ( c == operator_char ) {
                is_operator = true;
                break;
            }
        }            
        if ( c == ' ' || c == '\n' || c == ';' || is_operator) {
            if ( current_element == "" && !is_operator) {
                continue;
            }
            else {
                    bool is_reserved = false;
                    bool is_digit = true;
                if (current_element != "") {
                    for ( std::string word : reserved_words ) {
                        if ( current_element == word ) {
                            is_reserved = true;
                            bool is_digit = false;
                            break;
                        }
                    }
                    if (!not is_reserved) {
                        for ( char ch : current_element ) {
                            if ( std::isdigit(ch) == 0) {
                                bool is_digit = false;
                                break;
                            }
                        }
                    }
                    token newToken = setToken(current_element, is_reserved, false, is_digit);
                    tokens.push_back(newToken);
                    current_element = "";
                }
                if ( c == ';' ) {
                    token newToken;
                    newToken.type = tokenType::semicolon;
                    newToken.value = ";";
                    tokens.push_back(newToken);
                }
                else if (is_operator) {
                    token newToken = setToken(std::string(1, c), is_reserved = false, is_operator, is_digit = false);
                    tokens.push_back(newToken);
                }
            }
        }
        else {
            current_element += c;
        }
    }
    return tokens;
}