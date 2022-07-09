#pragma once
#include "lexer.cpp"
#include "astnode.cpp"

namespace parser
{
    using namespace std;
    
    class ParserError : public runtime_error
    {
    public:
        using runtime_error::runtime_error;
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
        OpInfo(int prec, Assoc assoc) : mPrec(prec), mAssoc(assoc) {}
        int getPrec() { return mPrec; }
        Assoc getAssoc() { return mAssoc; }
    };
    
    class Ast
    {
        lexer::Tokenizer mLexer;
        unique_ptr<astnode::AstNode> mRoot;
        lexer::Token mCurrToken;
        lexer::Token mPrevToken;
        vector<unique_ptr<astnode::AstNode>> mSubTrees;
        
        bool isAtEnd() {
            if (mCurrToken.getType() == lexer::TokenType::ENDOFFILE) {
                return true;
            }

            return false;
        }

        void advance() {
            mPrevToken = mCurrToken;
            mCurrToken = mLexer.next();
        }

        lexer::Token curr() {
            return mCurrToken;
        }

        lexer::Token previous() {
            return mPrevToken;
        }

        bool check(lexer::TokenType type) {
            if (type == mCurrToken.getType()) {
                return true;
            }

            return false;
        } 

        template<typename... T>
        bool match(T... types) {
            for (auto& type : {static_cast<lexer::TokenType>(types)...}) {
                if (check(type)) {
                    advance();
                    return true;
                }
            }

            return false;
        }

        OpInfo getPrecAndAssoc(string op) {
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

    public:
        Ast(lexer::Tokenizer lexer) : mLexer(lexer), mCurrToken(mLexer.next()), mPrevToken(mCurrToken) {}
        
        vector<unique_ptr<astnode::AstNode>> parse() {
            if (isAtEnd()) return move(mSubTrees);

            unique_ptr<astnode::AstNode> expr = parseExpr();

            mSubTrees.push_back(move(expr));

            return parse();
        }

        unique_ptr<astnode::AstNode> parseExpr(int minPrec = 0) {
            unique_ptr<astnode::AstNode> lhs = parsePrimary();

            while (!isAtEnd()) {
                string op = curr().getLexemeStr();
                auto opInfo = getPrecAndAssoc(op);

                if (opInfo.getPrec() == -1 || opInfo.getPrec() < minPrec) break;

                int nextMinPrec = opInfo.getAssoc() == Assoc::LEFT ? opInfo.getPrec() + 1 : opInfo.getPrec();

                advance();

                unique_ptr<astnode::AstNode> rhs = parseExpr(nextMinPrec);

                lhs = make_unique<astnode::BinaryNode>(astnode::BinaryNode(op, move(lhs), move(rhs)));
            }
                        
            mRoot = move(lhs);
            return move(mRoot);           
        }

        unique_ptr<astnode::AstNode> parsePrimary() {
            return parseNumber();
        }

        unique_ptr<astnode::AstNode> parseNumber() {
            if (match(lexer::TokenType::INT, lexer::TokenType::FLOAT)) {
                auto node = make_unique<astnode::NumberNode>(astnode::NumberNode(previous().getLexeme()));
                return node;
            }

            return parseIdentifier();
        }

        unique_ptr<astnode::AstNode> parseIdentifier() {
            if (match(lexer::TokenType::IDENTIFIER)) {
                auto node = make_unique<astnode::IdentifierNode>(astnode::IdentifierNode(previous().getLexeme()));
                return node;
            }

            return parsePrefix();
        }

        unique_ptr<astnode::AstNode> parsePrefix() {
            if (match(lexer::TokenType::PLUS, lexer::TokenType::MINUS)) {
                string op = previous().getLexemeStr();
                unique_ptr<astnode::AstNode> rhs = parsePrimary(); 
                return make_unique<astnode::PrefixNode>(astnode::PrefixNode(op, move(rhs)));
            }
            
            return parseParens();
        }

        unique_ptr<astnode::AstNode> parseParens() {
            if (match(lexer::TokenType::LEFT_PAREN)) {
                if (match(lexer::TokenType::RIGHT_PAREN)) return parseExpr();
                unique_ptr<astnode::AstNode> expr = parseExpr();
                if (match(lexer::TokenType::RIGHT_PAREN)) return expr;

                throw ParserError("Expected a closing ')' instead got" + curr().getLexemeStr() + "'");
            }

            throw ParserError("Expected a primary instead got '" + curr().getLexemeStr() + "'");
        }
    };
}