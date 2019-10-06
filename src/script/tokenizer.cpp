// SPDX-License-Identifier: GPL-3.0-only

#include "tokenizer.hpp"
#include <optional>

namespace Invader {
    std::vector<Tokenizer::Token> Tokenizer::tokenize(const char *string, bool &error, std::size_t &error_line, std::size_t &error_column, std::string &error_token) {
        // Token start
        const char *token_start = nullptr;
        std::size_t token_start_line = 0;
        std::size_t token_start_column = 0;
        bool escape = false;

        // The expected terminator of the token
        char expected_end = 0;

        std::vector<Token> tokens;

        // Go through each character until we hit a null terminator, then stop
        std::size_t line = 1;
        std::size_t column = 1;
        for(const char *c = string; *c; c++) {
            // Is it whitespace?
            bool whitespace = *c == ' ' || *c == '\t' || *c == '\r' || *c == '\n';

            // Is it a ( or ), and are we not in a string?
            bool parenthesis = (*c == '(' || *c == ')') && expected_end != '"';
            bool parenthesis_killed_the_radio_star = parenthesis && token_start && !escape;

            // Are we continuing a token?
            if(token_start != nullptr) {
                // Are we breaking a token? Make sure we aren't escaping (backslash) too
                if(!escape && ((expected_end == ' ' && whitespace) || expected_end == *c || parenthesis)) {
                    // Get the token string
                    std::size_t length = c - token_start;
                    std::string raw_string = std::string(c, length);

                    // Set these so we can determine if a string is numeric or not
                    bool numeric = true;
                    bool decimal = false;

                    // Start adding the token
                    auto &token = tokens.emplace_back();
                    token.line = token_start_line;
                    token.column = token_start_column;
                    expected_end = 0;
                    token_start = nullptr;

                    // Get everything in the string
                    std::string s;
                    bool t_escape = false;
                    for(char &c : raw_string) {
                        if(c == '\\' && !t_escape) {
                            t_escape = true;
                            numeric = false;
                        }
                        else {
                            if(t_escape) {
                                switch(c) {
                                    case 'n':
                                        s += '\n';
                                        break;
                                    case 'r':
                                        s += '\r';
                                        break;
                                    case 't':
                                        s += '\t';
                                        break;
                                    default:
                                        s += c;
                                        break;
                                }
                            }
                            else {
                                // Check to see if we are making something not numeric
                                if((c < '0' || c > '9') && (c != '-' || s.size() > 0)) {
                                    // If it's a decimal, we aren't a decimal already, and the next character isn't a null terminator, set decimal to true
                                    if(c == '.' && *(&c + 1) != 0 && decimal == false) {
                                        decimal = true;
                                    }
                                    else {
                                        numeric = false;
                                    }
                                }
                                s += c;
                            }
                            t_escape = false;
                        }
                    }

                    // Remove the parenthesis if needed
                    if(raw_string[0] == '"') {
                        s = s.substr(1, s.size() - 1);
                    }

                    // If it's non-numeric, set the value as is
                    if(!numeric) {
                        token.value = s;
                        token.type = TokenType::TOKEN_TYPE_STRING;
                    }

                    // If it's a decimal, let's see it
                    else if(decimal) {
                        token.value = static_cast<float>(std::strtof(s.data(), nullptr));
                        token.type = TokenType::TOKEN_TYPE_DECIMAL;
                    }

                    // Otherwise, if it's an integer, let's see it
                    else {
                        token.value = static_cast<std::int32_t>(std::strtol(s.data(), nullptr, 10));
                        token.type = TokenType::TOKEN_TYPE_INTEGER;
                    }
                }
            }

            // Otherwise, if it isn't a whitespace, we're starting a token
            else if(!whitespace) {
                // If it's a parenthesis, it's only one character long
                if(parenthesis) {
                    auto &token = tokens.emplace_back();
                    token.line = line;
                    token.column = column;
                    token.value = std::string(c, 1);
                    token.type = *c == '(' ? TokenType::TOKEN_TYPE_PARENTHESIS_BEGIN : TokenType::TOKEN_TYPE_PARENTHESIS_END;
                }

                // If it's the start of a comment, end the line
                else if(*c == ';') {
                    while(*c != '\n') {
                        c++;
                    }
                }

                // Otherwise it's the start of a token
                else {
                    token_start = c;
                    if(*c == '"') {
                        expected_end = '"';
                    }
                    else {
                        expected_end = ' ';
                    }
                    token_start_column = column;
                    token_start_line = line;
                }
            }

            // If it's parenthesis, let's redo this
            if(parenthesis_killed_the_radio_star) {
                c--;
                continue;
            }

            // If we used a backslash, we're escaping the next character
            if(*c == '\\' && !escape) {
                escape = true;
            }
            else {
                escape = false;
            }

            // Increment the line and column counter
            if(*c == '\n') {
                line++;
                column = 1;
            }
            else {
                column++;
            }
        }

        // if we're ending while still in the middle of a token, then error
        if(token_start) {
            error = true;
            error_line = token_start_line;
            error_column = token_start_column;
            error_token = token_start;
        }
        else {
            error = false;
        }

        return tokens;
    }
}
