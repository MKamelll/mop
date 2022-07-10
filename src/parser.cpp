#include "lexer.h"
#include "parser.h"
#include "astnode.h"

using namespace parser;

/////////////////////////////////////////////////////////////////////

OpInfo::OpInfo(int prec, Assoc assoc) : mPrec(prec), mAssoc(assoc) {}

int OpInfo::getPrec() { return mPrec; }

Assoc OpInfo::getAssoc() { return mAssoc; }

///////////////////////////////////////////////////////////////////

Ast::Ast(lexer::Tokenizer lexer) : mLexer(lexer), mCurrToken(mLexer.next()), mPrevToken(mCurrToken) {}

bool Ast::isAtEnd() {
    if (mCurrToken.getType() == lexer::TokenType::ENDOFFILE) {
        return true;
    }

    return false;
}

void Ast::advance() {
    mPrevToken = mCurrToken;
    mCurrToken = mLexer.next();
}

lexer::Token Ast::curr() {
    return mCurrToken;
}

lexer::Token Ast::previous() {
    return mPrevToken;
}

bool Ast::check(lexer::TokenType type) {
    if (type == mCurrToken.getType()) {
        return true;
    }

    return false;
} 

template<typename... T>
bool Ast::match(T... types) {
    for (auto& type : {static_cast<lexer::TokenType>(types)...}) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

OpInfo Ast::getPrecAndAssoc(std::string op) {
    if (op == "+") {
        return OpInfo(4, Assoc::LEFT);
    } else if (op == "-") {
        return OpInfo(4, Assoc::LEFT);
    } else if (op == "*") {
        return OpInfo(5, Assoc::LEFT);
    } else if (op == "/") {
        return OpInfo(5, Assoc::LEFT);
    } else if (op == "%") {
        return OpInfo(5, Assoc::LEFT);
    } else if (op == "^") {
        return OpInfo(6, Assoc::RIGHT);
    } else {
        return OpInfo(-1, Assoc::NONE);
    }
}
        
std::vector<std::unique_ptr<astnode::AstNode>> Ast::parse() {
    if (isAtEnd()) return std::move(mSubTrees);

    std::unique_ptr<astnode::AstNode> expr = parseExpr();

    mSubTrees.push_back(std::move(expr));

    return parse();
}

std::unique_ptr<astnode::AstNode> Ast::parseExpr(int minPrec) {
    std::unique_ptr<astnode::AstNode> lhs = parsePrimary();

    while (!isAtEnd()) {
        std::string op = curr().getLexemeStr();
        auto opInfo = getPrecAndAssoc(op);

        if (opInfo.getPrec() == -1 || opInfo.getPrec() < minPrec) break;

        int nextMinPrec = opInfo.getAssoc() == Assoc::LEFT ? opInfo.getPrec() + 1 : opInfo.getPrec();

        advance();

        std::unique_ptr<astnode::AstNode> rhs = parseExpr(nextMinPrec);

        lhs = std::make_unique<astnode::BinaryNode>(astnode::BinaryNode(op, std::move(lhs), std::move(rhs)));
    }
                
    mRoot = std::move(lhs);
    return std::move(mRoot);           
}

std::unique_ptr<astnode::AstNode> Ast::parsePrimary() {
    return parseNumber();
}

std::unique_ptr<astnode::AstNode> Ast::parseNumber() {
    if (match(lexer::TokenType::INT, lexer::TokenType::FLOAT)) {
        return std::make_unique<astnode::NumberNode>(astnode::NumberNode(previous().getLexeme()));
    }

    return parseIdentifier();
}

std::unique_ptr<astnode::AstNode> Ast::parseIdentifier() {
    if (match(lexer::TokenType::IDENTIFIER)) {
        return std::make_unique<astnode::IdentifierNode>(astnode::IdentifierNode(previous().getLexeme()));
    }

    return parsePrefix();
}

std::unique_ptr<astnode::AstNode> Ast::parsePrefix() {
    if (match(lexer::TokenType::PLUS, lexer::TokenType::MINUS)) {
        std::string op = previous().getLexemeStr();
        std::unique_ptr<astnode::AstNode> rhs = parsePrimary(); 
        return std::make_unique<astnode::PrefixNode>(astnode::PrefixNode(op, std::move(rhs)));
    }
    
    return parseParens();
}

std::unique_ptr<astnode::AstNode> Ast::parseParens() {
    if (match(lexer::TokenType::LEFT_PAREN)) {
        if (match(lexer::TokenType::RIGHT_PAREN)) return parseExpr();
        std::unique_ptr<astnode::AstNode> expr = parseExpr();
        if (match(lexer::TokenType::RIGHT_PAREN)) return expr;

        throw ParserError("Expected a closing ')' instead got" + curr().getLexemeStr() + "'");
    }

    throw ParserError("Expected a primary instead got '" + curr().getLexemeStr() + "'");
}