#pragma once
#include "lexer.h"
#include "astnode.h"

namespace parser {   

class ParserError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

enum class Assoc
{
    RIGHT, LEFT, NONE
};

class OpInfo
{
    int mPrec;
    Assoc mAssoc;
public:
    OpInfo(int prec, Assoc assoc);
    int getPrec();
    Assoc getAssoc();
};

class Ast
{
    lexer::Tokenizer mLexer;
    std::unique_ptr<astnode::AstNode> mRoot;
    lexer::Token mCurrToken;
    lexer::Token mPrevToken;
    std::vector<std::unique_ptr<astnode::AstNode>> mSubTrees;
    
    bool isAtEnd();
    void advance();
    lexer::Token curr();
    lexer::Token previous();
    bool check(lexer::TokenType type);

    template<typename... T>
    bool match(T... types);

    OpInfo getPrecAndAssoc(std::string op);

public:
    Ast(lexer::Tokenizer lexer);
    
    std::vector<std::unique_ptr<astnode::AstNode>> parse();
    std::unique_ptr<astnode::AstNode> parseExpr(int minPrec = 0);
    std::unique_ptr<astnode::AstNode> parsePrimary();
    std::unique_ptr<astnode::AstNode> parseNumber();
    std::unique_ptr<astnode::AstNode> parseIdentifier();
    std::unique_ptr<astnode::AstNode> parsePrefix();
    std::unique_ptr<astnode::AstNode> parseParens();
};

}