#pragma once

#include <string>
#include <vector>

enum class TokenType
{
    _exit,
    int_lit,
    semi,
};

struct Token
{
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer
{
    public:
        inline explicit Tokenizer(const std::string source)
            : m_source(std::move(source))
        {

        }

        inline std::vector<Token> tokenize()
        {
            std::vector<Token> tokens;
            std::string buf;

            while(peak().has_value())
            {
                if(std::isalpha(peak().value()))
                {
                    buf.push_back(consume());

                    while(peak().has_value() && std::isalnum(peak().value()))
                    {
                        buf.push_back(consume());
                    }

                    if(buf == "exit")
                    {
                        tokens.push_back({.type = TokenType::_exit});
                        buf.clear();
                        continue;
                    } else {
                        std::cerr << "Invalid syntax. Read the docs again or check your spelling!" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                else if(std::isdigit(peak().value()))
                {
                    buf.push_back(consume());
                    while(peak().has_value() && std::isdigit(peak().value()))
                    {
                        buf.push_back(consume());
                    }

                    tokens.push_back({.type = TokenType::int_lit, .value = buf});
                    buf.clear();

                    continue;
                }
                else if(peak().value() == ';')
                {
                    consume();
                    tokens.push_back({.type = TokenType::semi});
                    continue;
                }
                else if(std::isspace(peak().value()))
                {
                    consume();
                    continue;
                }
                else
                {
                    std::cerr << "Invalid syntax. Read the docs again or check your spelling!" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            m_index = 0;

            return tokens;
        }

    private:
        [[nodiscard]] std::optional<char> peak(int ahead = 1) const
        {
            if(m_index + ahead > m_source.length())
                return {};
            else
                return m_source.at(m_index);
        }

        inline char consume()
        {
            return m_source.at(m_index++);
        }

        const std::string m_source;
        int m_index = 0;
};