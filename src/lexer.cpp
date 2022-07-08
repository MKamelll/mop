#pragma once
#include<string>
#include<iostream>
#include<variant>
#include<exception>
#include<stdexcept>
#include<cctype>
#include<sstream>

namespace Lexer
{
    using namespace std;

    class LexerError : public runtime_error
    {   
    
    public:
        using runtime_error::runtime_error;
    };

    enum class TokenType
    {
        LEFT_PAREN, RIGHT_PAREN, PLUS, MINUS, STAR, SLASH, MOD, CARROT, INT, FLOAT, IDENTIFIER, ENDOFFILE
    };

    ostream & operator<<(ostream & str, TokenType type) {
        switch (type) {
            case TokenType::LEFT_PAREN: str << "LEFT_PAREN"; break;
            case TokenType::RIGHT_PAREN: str << "RIGHT_PAREN"; break;
            case TokenType::PLUS: str << "PLUS"; break;
            case TokenType::MINUS: str << "MINUS"; break;
            case TokenType::STAR: str << "STAR"; break;
            case TokenType::SLASH: str << "SLASH"; break;
            case TokenType::INT: str << "INT"; break;
            case TokenType::FLOAT: str << "FLOAT"; break;
            case TokenType::IDENTIFIER: str << "IDENTIFIER"; break;
            case TokenType::MOD: str << "MOD"; break;
            case TokenType::CARROT: str << "CARROT"; break;
            case TokenType::ENDOFFILE: str << "ENDOFFILE"; break;
            default:
                throw LexerError("Undefined TokenType");
        }

        return str;
    }

    class Lexeme
    {
        variant<string, int, float> mLexeme;
    
    public:
        Lexeme(string str) : mLexeme(str) {}
        Lexeme(int num) : mLexeme(num) {}
        Lexeme(float num) : mLexeme(num) {}

        string getType() {
            if (holds_alternative<string>(mLexeme)) {
                return "string";
            } else if (holds_alternative<int>(mLexeme)) {
                return "int";
            } else if (holds_alternative<float>(mLexeme)) {
                return "float";
            }

            throw LexerError("Unknown token type");
        }

        variant<string, int, float> getLexeme() {
            return mLexeme;
        }

        string getLexemeStr() {
            if (getType() == "int") {
                return to_string(get<int>(mLexeme));
            } else if (getType() == "float") {
                return to_string(get<float>(mLexeme));
            }

            return get<string>(mLexeme);
        }

        friend ostream & operator<<(ostream & str, Lexeme lexeme) {
            if (lexeme.getType() == "string") {
                str << "'" << get<string>(lexeme.mLexeme) << "'";
            } else if (lexeme.getType() == "int") {
                str << get<int>(lexeme.mLexeme);
            } else if (lexeme.getType() == "float") {
                str << get<float>(lexeme.mLexeme);
            }

            return str;
        }
    };

    class Token
    {
        TokenType mType;
        Lexeme mLexeme;
    
    public:    
        Token(TokenType type, Lexeme lexeme) : mType(type), mLexeme(lexeme) {}

        TokenType getType() {
            return mType;
        }

        Lexeme getLexeme() {
            return mLexeme;
        }

        string getLexemeStr() {
            return mLexeme.getLexemeStr();
        }

        friend ostream & operator<<(ostream & str, Token token) {
            str << "(type: " << token.mType << ", lexeme: " << token.mLexeme << ")";
            return str;
        }
    };
    
    class Tokenizer
    {
        string mSrc;
        size_t mCurrIndex;
        char mCurrChar;
    
    private:    
        void advance(int step = 1) {
            mCurrIndex+=step;
        }

        bool isAtEnd() {
            if (mCurrIndex < mSrc.length()) return false;

            return true;
        }

        char previous() {
            return mSrc[mCurrIndex - 1];
        }

        char curr() {
            return mSrc[mCurrIndex];
        }
        
    public:
        Tokenizer(string source) : mSrc(source), mCurrIndex(0) {}

        Token next() {
            if (isAtEnd()) return Token(TokenType::ENDOFFILE, Lexeme("EOF"));
            switch (curr())
            {
                case '+': {
                    advance();
                    return Token(TokenType::PLUS, Lexeme("+"));
                }
                case '-': {
                    advance();
                    return Token(TokenType::MINUS, Lexeme("-"));
                } 
                case '*': {
                    advance();
                    return Token(TokenType::STAR, Lexeme("*"));
                } 
                case '/': {
                    advance();
                    return Token(TokenType::SLASH, Lexeme("/"));
                }
                case '(': {
                    advance();
                    return Token(TokenType::LEFT_PAREN, Lexeme("("));
                }
                case ')': {
                    advance();
                    return Token(TokenType::RIGHT_PAREN, Lexeme(")"));
                }
                case '%': {
                    advance();
                    return Token(TokenType::MOD, Lexeme("%"));
                } 
                case '^': {
                    advance();
                    return Token(TokenType::CARROT, Lexeme("^"));
                }
                
                default: {
                    if (isdigit(curr())) {
                        variant<int, float> number = aNumber();

                        if (holds_alternative<int>(number)) {
                            return Token(TokenType::INT, Lexeme(get<int>(number)));
                        } else if (holds_alternative<float>(number)) {
                            return Token(TokenType::FLOAT, Lexeme(get<float>(number)));
                        }
                    } else if (isalpha(curr())) {
                        string identifier = anIdentifier();
                        return Token(TokenType::IDENTIFIER, Lexeme(identifier));
                    }
                    
                    advance();
                    return next();
                }
            }
        }

        variant<int, float> aNumber() {
            stringstream result("");
            bool isFloat = false;
            
            while (!isAtEnd()) {
                if (curr() == '.') {
                    isFloat = true;
                    result << curr();
                    advance();
                    continue;
                }
                if (!isdigit(curr())) break;

                result << curr();
                advance();
            }

            if (!isFloat) {
                return stoi(result.str());
            }

            return stof(result.str());
        }

        string anIdentifier() {
            stringstream result("");

            while (!isAtEnd()) {
                if (!isalnum(curr())) break;
                result << curr();
                advance();
            }

            return result.str();
        }
    };
}