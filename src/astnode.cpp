#pragma once
#include "lexer.cpp"

namespace astnode
{
    using namespace std;

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
        PrimaryNode(lexer::Lexeme value) : mValue(value.getLexeme()) {}

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

}