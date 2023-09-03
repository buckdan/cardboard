#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>

#include "./tokenization.hpp"


std::string tokens_to_asm(std::vector<Token>& tokens)
{
    std::stringstream output;
    output << "global _start\nstart:\n";
    for(int i = 0; i < tokens.size(); i++)
    {  
        const Token token = tokens.at(i);
        if(token.type == TokenType::_exit)
        {
            if(i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit)
            {
                if(i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi)
                {
                    // Make sure to indent 4 times (a.k.a press space 4 time)
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
                    output << "    syscall";
                }
            }
        }
    }

    return output.str();
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cerr << "fatal error : no input files" << std::endl;
        std::cout << "cardboard <file>.cb" << std::endl;

        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();
    {
        std::fstream outputFile("./out.asm", std::ios::out);
        outputFile << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}