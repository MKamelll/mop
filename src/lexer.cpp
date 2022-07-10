#include "lexer.h"

namespace lexer {

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

std::ostream & operator<<(std::ostream & str, Lexeme lexeme) {
    if (lexeme.getType() == "string") {
        str << "'" << std::get<std::string>(lexeme.mLexeme) << "'";
    } else if (lexeme.getType() == "int") {
        str << std::get<int>(lexeme.mLexeme);
    } else if (lexeme.getType() == "float") {
        str << std::get<float>(lexeme.mLexeme);
    }
    return str;
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

std::ostream & operator<<(std::ostream & str, Token token) {
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

}
