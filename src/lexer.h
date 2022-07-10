#pragma once
#include "common.h"

namespace lexer {

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
    friend std::ostream & operator<<(std::ostream & str, Lexeme lexeme);
};

std::ostream & operator<<(std::ostream & str, Lexeme lexeme);

class Token
{
    TokenType mType;
    Lexeme mLexeme;

public:    
    Token(TokenType type, Lexeme lexeme);

    TokenType getType();
    Lexeme getLexeme();
    std::string getLexemeStr();
    friend std::ostream & operator<<(std::ostream & str, Token token);
};

std::ostream & operator<<(std::ostream & str, Token token);

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