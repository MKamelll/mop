#pragma once
#include "lexer.cpp"
#include<vector>
#include<memory>
#include<type_traits>

namespace Parser
{
    using namespace std;
    
    class ParserError : public runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class AstNode {
    public:
        virtual void print(ostream & str) = 0;
        friend ostream & operator<<(ostream & str, unique_ptr<AstNode> & node) {
            node->print(str);
            return str;
        }
    };

    class PrimaryNode : public AstNode
    {
        variant<string, int, float> mValue;
    
    protected:
        void print(ostream & str) override {
            if (getType() == "string") {
                str << "Identifier(" << get<string>(mValue) << ")";
            } else if (getType() == "int") {
                str << "Int(" << get<int>(mValue) << ")";
            } else if (getType() == "float") {
                str << "Float(" << get<float>(mValue) << ")";
            }
        }
    
    public:
        PrimaryNode(variant<string, int, float> value) : mValue(value) {}
        PrimaryNode(Lexer::Lexeme value) : mValue(value.getLexeme()) {}

        string getType() {
            if (holds_alternative<int>(mValue)) {
                return "int";
            } else if (holds_alternative<float>(mValue)) {
                return "float";
            }
            return "string";
        }
    };

    class NumberNode : public PrimaryNode {
        using PrimaryNode::PrimaryNode;
    };
    class IdentifierNode : public PrimaryNode {
        using PrimaryNode::PrimaryNode;
    };

    class BinaryNode : public AstNode
    {
        string mOp;
        unique_ptr<AstNode> mLhs;
        unique_ptr<AstNode> mRhs;

        void print(ostream & str) override {
            str << "Binary(op: '" << mOp << "', lhs: " << mLhs << ", rhs: " << mRhs << ")";
        }
    
    public:
        BinaryNode(string op, unique_ptr<AstNode> lhs, unique_ptr<AstNode> rhs) 
            : mOp(op), mLhs(move(lhs)), mRhs(move(rhs)) {}
    };

    class PrefixNode : public AstNode
    {
        string mOp;
        unique_ptr<AstNode> mRhs;

        void print(ostream & str) override {
            str << "Prefix(op: '" << mOp << "', rhs: " << mRhs << ")";
        }
    
    public:
        PrefixNode(string op, unique_ptr<AstNode> rhs) : mOp(op), mRhs(move(rhs)) {}
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
        Lexer::Tokenizer mLexer;
        unique_ptr<AstNode> mRoot;
        Lexer::Token mCurrToken;
        Lexer::Token mPrevToken;
        vector<unique_ptr<AstNode>> mSubTrees;
        
        bool isAtEnd() {
            if (mCurrToken.getType() == Lexer::TokenType::ENDOFFILE) {
                return true;
            }

            return false;
        }

        void advance() {
            mPrevToken = mCurrToken;
            mCurrToken = mLexer.next();
        }

        Lexer::Token curr() {
            return mCurrToken;
        }

        Lexer::Token previous() {
            return mPrevToken;
        }

        bool check(Lexer::TokenType type) {
            if (type == mCurrToken.getType()) {
                return true;
            }

            return false;
        } 

        template<typename... T>
        bool match(T... types) {
            for (auto& type : {static_cast<Lexer::TokenType>(types)...}) {
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
        Ast(Lexer::Tokenizer lexer) : mLexer(lexer), mCurrToken(mLexer.next()), mPrevToken(mCurrToken) {}
        
        vector<unique_ptr<AstNode>> parse() {
            if (isAtEnd()) return move(mSubTrees);

            unique_ptr<AstNode> expr = parseExpr();

            mSubTrees.push_back(move(expr));

            return parse();
        }

        unique_ptr<AstNode> parseExpr(int minPrec = 0) {
            unique_ptr<AstNode> lhs = parsePrimary();

            while (!isAtEnd()) {
                string op = curr().getLexemeStr();
                auto opInfo = getPrecAndAssoc(op);

                if (opInfo.getPrec() == -1 || opInfo.getPrec() < minPrec) break;

                int nextMinPrec = opInfo.getAssoc() == Assoc::LEFT ? opInfo.getPrec() + 1 : opInfo.getPrec();

                advance();

                unique_ptr<AstNode> rhs = parseExpr(nextMinPrec);

                lhs = make_unique<BinaryNode>(BinaryNode(op, move(lhs), move(rhs)));
            }
                        
            mRoot = move(lhs);
            return move(mRoot);           
        }

        unique_ptr<AstNode> parsePrimary() {
            return parseNumber();
        }

        unique_ptr<AstNode> parseNumber() {
            if (match(Lexer::TokenType::INT, Lexer::TokenType::FLOAT)) {
                auto node = make_unique<NumberNode>(NumberNode(previous().getLexeme()));
                return node;
            }

            return parseIdentifier();
        }

        unique_ptr<AstNode> parseIdentifier() {
            if (match(Lexer::TokenType::IDENTIFIER)) {
                auto node = make_unique<IdentifierNode>(IdentifierNode(previous().getLexeme()));
                return node;
            }

            return parsePrefix();
        }

        unique_ptr<AstNode> parsePrefix() {
            if (match(Lexer::TokenType::PLUS, Lexer::TokenType::MINUS)) {
                string op = previous().getLexemeStr();
                unique_ptr<AstNode> rhs = parsePrimary(); 
                return make_unique<PrefixNode>(PrefixNode(op, move(rhs)));
            }
            
            return parseParens();
        }

        unique_ptr<AstNode> parseParens() {
            if (match(Lexer::TokenType::LEFT_PAREN)) {
                if (match(Lexer::TokenType::RIGHT_PAREN)) return parseExpr();
                unique_ptr<AstNode> expr = parseExpr();
                if (match(Lexer::TokenType::RIGHT_PAREN)) return expr;

                throw ParserError("Expected a closing ')' instead got" + curr().getLexemeStr() + "'");
            }

            throw ParserError("Expected a primary instead got '" + curr().getLexemeStr() + "'");
        }
    };
}