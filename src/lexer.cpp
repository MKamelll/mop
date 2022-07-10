#include "lexer.h"

using namespace lexer;

///////////////////////////////////////////////////////////////////
Lexeme::Lexeme(std::string str) : mLexeme(str) {}
Lexeme::Lexeme(int num) : mLexeme(num) {}
Lexeme::Lexeme(float num) : mLexeme(num) {}

std::string Lexeme::getType() {
    if (std::holds_alternative<std::string>(mLexeme)) {
        return "std::string";
    } else if (std::holds_alternative<int>(mLexeme)) {
        return "int";
    } else if (std::holds_alternative<float>(mLexeme)) {
        return "float";
    }

    throw LexerError("Unknown token type");
}

std::variant<std::string, int, float> Lexeme::getLexeme() {
    return mLexeme;
}

std::string Lexeme::getLexemeStr() {
    if (getType() == "int") {
        return std::to_string(std::get<int>(mLexeme));
    } else if (getType() == "float") {
        return std::to_string(std::get<float>(mLexeme));
    }

    return std::get<std::string>(mLexeme);
}

/////////////////////////////////////////////////////////////////////////////
Token::Token(TokenType type, Lexeme lexeme) : mType(type), mLexeme(lexeme) {}

TokenType Token::getType() {
    return mType;
}

Lexeme Token::getLexeme() {
    return mLexeme;
}

std::string Token::getLexemeStr() {
    return mLexeme.getLexemeStr();
}

//////////////////////////////////////////////////////////////////////////

Tokenizer::Tokenizer(std::string source) : mSrc(source), mCurrIndex(0) {}

void Tokenizer::advance(int step) {
    mCurrIndex+=step;
}

bool Tokenizer::isAtEnd() {
    if (mCurrIndex < mSrc.length()) return false;

    return true;
}

char Tokenizer::previous() {
    return mSrc[mCurrIndex - 1];
}

char Tokenizer::curr() {
    return mSrc[mCurrIndex];
}

Token Tokenizer::next() {
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
                std::variant<int, float> number = aNumber();

                if (std::holds_alternative<int>(number)) {
                    return Token(TokenType::INT, Lexeme(std::get<int>(number)));
                } else if (std::holds_alternative<float>(number)) {
                    return Token(TokenType::FLOAT, Lexeme(std::get<float>(number)));
                }
            } else if (isalpha(curr())) {
                std::string identifier = anIdentifier();
                return Token(TokenType::IDENTIFIER, Lexeme(identifier));
            }
            
            advance();
            return next();
        }
    }
}

std::variant<int, float> Tokenizer::aNumber() {
    std::stringstream result("");
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

std::string Tokenizer::anIdentifier() {
    std::stringstream result("");

    while (!isAtEnd()) {
        if (!isalnum(curr())) break;
        result << curr();
        advance();
    }

    return result.str();
}