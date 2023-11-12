#include <iostream>
#include <fstream>
#include <vector>


// Definitions

std::string reserved_words[] { "return", "int", "float", ";"};

enum class tokenType {
    return_kw,
    int_type,
    float_type,
    int_value,
    float_value,
    identifier,

    equal_operator,
    semicolon,
};

struct identifier {
    std::string name;
    std::string value;
    tokenType type;
};

struct token {
    tokenType type;
    std::string value;
    std::string name;
};

std::string read_file(std::string file_name);
std::vector<token> lexicalAnalysis(std::string file_contents);
std::vector<token> setValues(std::vector<token> tokens);
std::string assembly_generator(std::vector<token> tokens);



//main code

int main(int argc, char* argv[]) {
    if ( argc != 2 ) {
        std::cout << "usage: Mellow <name.c>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string file_contents;
    file_contents = read_file(argv[1]);

    std::cout << file_contents << std::endl;
    std::vector<token> tokens = lexicalAnalysis(file_contents);

    std::cout << "hi1" << std::endl;

    tokens = setValues(tokens);

    std::cout << "hi2" << std::endl;

    for ( token t : tokens ) {
        std::cout << t.value << std::endl;
    }
}


//Functions

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
        newToken.type = tokenType::equal_operator;
        newToken.value = value;      
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
            newToken.name = value;
        }

    return newToken;
}

std::vector<token> lexicalAnalysis(std::string file_contents) {
    std::vector<token> tokens;
    std::string current_element;
    for ( char c : file_contents ) {
        bool is_operator = false;
                  
        if ( c == ' ' || c == '\n' || c == ';' || c == '=') {
            if ( current_element == "" &&  c != '=') {
                continue;
            }
            else {
                    bool is_reserved = false;
                    bool is_digit = true;
                if (current_element != "") {
                    for ( std::string word : reserved_words ) {
                        if ( current_element == word ) {
                            is_reserved = true;
                            is_digit = false;
                            break;
                        }
                    }
                    if (!is_reserved) {
                        for ( char ch : current_element ) {
                            if ( std::isdigit(ch) == 0) {
                                is_digit = false;
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
                else if (c == '=') {
                    token newToken = setToken(std::string(1, c), is_reserved = false, true, is_digit = false);
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

std::vector<token> setValues(std::vector<token> tokens) {
    std::vector<token> return_tokens;
    std::vector<identifier> initialized_identifiers;

    bool initialized = false;
    int length = tokens.size();
    int statements {};

    for (int i = 0; i < length; i++) {
        token current_token = tokens[i];
        if(current_token.type == tokenType::int_type || current_token.type == tokenType::float_type) {
            if (tokens[i+1].type == tokenType::identifier) {
                for (identifier id : initialized_identifiers) {
                    if (id.name == tokens[i+1].value) {
                        std::cout << "Error: Identifier already initialized" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                i++;
                if (tokens[i+1].type == tokenType::equal_operator) {
                    if (tokens[i+2].type == tokenType::int_value || tokens[i+2].type == tokenType::float_value) {
                        if (tokens[i+3].type == tokenType::semicolon) {
                            identifier new_identifier;
                            new_identifier.name = tokens[i].value;
                            new_identifier.type = tokens[i].type;
                            new_identifier.value = tokens[i+2].value;
                            initialized_identifiers.push_back(new_identifier);
                            
                            tokens[i].value = tokens[i+2].value;
                            return_tokens.push_back(current_token);
                            return_tokens.push_back(tokens[i]);
                        }
                        else {
                            std::cout << "Error: Expected semicolon after value" << std::endl;
                            exit(EXIT_FAILURE);
                        }  
                    }
                    else {
                        std::cout << "Error: Expected value after equal operator" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                else if (tokens[i+1].type == tokenType::semicolon) {
                    identifier new_identifier;
                    new_identifier.name = tokens[i].value;
                    new_identifier.type = current_token.type;
                    initialized_identifiers.push_back(new_identifier);

                    tokens[i].value = "0";
                    return_tokens.push_back(current_token);
                    return_tokens.push_back(tokens[i]);
                }
                else {
                    std::cout << "Error: Expected equal operator or semicolon after identifier" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else {
                std::cout << "Error: Expected identifier after type declaration" << std::endl;
                exit(EXIT_FAILURE);
            }
            
        }
        else if (current_token.type == tokenType::identifier) {
            for (identifier id : initialized_identifiers) {
                    if (id.name == current_token.name) {
                        initialized = true;
                    }
                }
            if (!initialized) {
                std::cout << "Error: non-initialized identifier" << std::endl;
                exit(EXIT_FAILURE);
            }
            
            if (tokens[i+1].type == tokenType::equal_operator) {
                if (tokens[i+2].type == tokenType::int_value || tokens[i+2].type == tokenType::float_value) {
                    if (tokens[i+3].type == tokenType::semicolon) {
                        tokens[i].value = tokens[i+2].value;
                        return_tokens.push_back(current_token);
                    }
                    else {
                        std::cout << "Error: Expected semicolon after value" << std::endl;
                        exit(EXIT_FAILURE);
                    }  
                }
                else {
                    std::cout << "Error: Expected value after equal operator" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if (tokens[i+1].type == tokenType::semicolon) {
                return_tokens.push_back(current_token);
                std::cout << "warning! unused variable" << std::endl;
            }
            else {
                std::cout << "Error: Expected equal operator or semicolon after identifier" << std::endl;
                exit(EXIT_FAILURE);
            }
            
        }
        else {
            return_tokens.push_back(current_token);
        }
    }

    return return_tokens;
}


std::string assembly_generator(std::vector<token> tokens) {
    std::string assembly_code;
    int length = tokens.size();
    for (int i = 0; i < length; i++) {
        token current_token = tokens[i];
    }
}