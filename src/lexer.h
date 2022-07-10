#pragma once
#include "common.h"

namespace lexer
{
    class LexerError : public std::runtime_error
    {   
    
    public:
        using runtime_error::runtime_error;
    };

    enum class TokenType
    {
        LEFT_PAREN, RIGHT_PAREN, PLUS, MINUS, STAR, SLASH, MOD, CARROT, INT, FLOAT, IDENTIFIER, ENDOFFILE
    };

    class Lexeme
    {
        std::variant<std::string, int, float> mLexeme;
    
    public:
        Lexeme(std::string str);
        Lexeme(int num);
        Lexeme(float num);

        std::string getType();
        std::variant<std::string, int, float> getLexeme();
        std::string getLexemeStr();
        friend std::ostream & operator<<(std::ostream & str, Lexeme lexeme) {
            if (lexeme.getType() == "string") {
                str << "'" << std::get<std::string>(lexeme.mLexeme) << "'";
            } else if (lexeme.getType() == "int") {
                str << std::get<int>(lexeme.mLexeme);
            } else if (lexeme.getType() == "float") {
                str << std::get<float>(lexeme.mLexeme);
            }
            return str;
        }
    };

    class Token
    {
        TokenType mType;
        Lexeme mLexeme;
    
    public:    
        Token(TokenType type, Lexeme lexeme);

        TokenType getType();
        Lexeme getLexeme();
        std::string getLexemeStr();
        friend std::ostream & operator<<(std::ostream & str, Token token) {
            str << "(type: ";
            switch (token.mType) {
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
            
            str << ", lexeme: " << token.mLexeme << ")";
            return str;
        }
    };
    
    class Tokenizer
    {
        std::string mSrc;
        size_t mCurrIndex;
        char mCurrChar;
    
    private:    
        void advance(int step = 1);
        bool isAtEnd();
        char previous();
        char curr();
        
    public:
        Tokenizer(std::string source);
        Token next();
        std::variant<int, float> aNumber();
        std::string anIdentifier();
    };
}