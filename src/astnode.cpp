#pragma once
#include "lexer.h"

namespace astnode
{
    using namespace std;

    class PrimaryNode;
    class BinaryNode;
    class PrefixNode;

    class ToStringVisitor
    {
    public:
        string visit(PrimaryNode * node);
        string visit(BinaryNode * node);
        string visit(PrefixNode * node);
    };

    class AstNode {
    public:
        virtual string accept(unique_ptr<ToStringVisitor> visitor) = 0;
        friend ostream & operator<<(ostream & str, AstNode & node) {
            str << node.accept(make_unique<ToStringVisitor>(ToStringVisitor()));
            return str;
        }
    };

    class PrimaryNode : public AstNode
    {
        variant<string, int, float> mValue;
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

        auto getValue() {
            return mValue;
        }

        string accept(unique_ptr<ToStringVisitor> visitor) override {
            return visitor->visit(this);
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
    
    public:
        BinaryNode(string op, unique_ptr<AstNode> lhs, unique_ptr<AstNode> rhs) 
            : mOp(op), mLhs(move(lhs)), mRhs(move(rhs)) {}
        
        string getOp() {
            return mOp;
        }

        AstNode & getLhs() {
            return *mLhs;
        }

        AstNode & getRhs() {
            return *mRhs;
        }

        string accept(unique_ptr<ToStringVisitor> visitor) override {
            return visitor->visit(this);
        }
    };

    class PrefixNode : public AstNode
    {
        string mOp;
        unique_ptr<AstNode> mRhs;
    
    public:
        PrefixNode(string op, unique_ptr<AstNode> rhs) : mOp(op), mRhs(move(rhs)) {}

        string getOp() {
            return mOp;
        }

        AstNode & getRhs() {
            return *mRhs;
        }

        string accept(unique_ptr<ToStringVisitor> visitor) override {
            return visitor->visit(this);
        }
    };

    string ToStringVisitor::visit(PrimaryNode * node) {
            stringstream str;
            if (node->getType() == "string") {
                str << "Identifier(" << get<string>(node->getValue()) << ")";
            } else if (node->getType() == "int") {
                str << "Int(" << get<int>(node->getValue()) << ")";
            } else if (node->getType() == "float") {
                str << "Float(" << get<float>(node->getValue()) << ")";
            }

            return str.str();
        }

    string ToStringVisitor::visit(BinaryNode * node) {
            stringstream str;
            str << "Binary(op: '" << node->getOp() << "', lhs: " << node->getLhs() << ", rhs: " << node->getRhs() << ")";
            return str.str();
        }

    string ToStringVisitor::visit(PrefixNode * node) {
            stringstream str;
            str << "Prfix(op: " << node->getOp() << ", rhs: " << node->getRhs() << ")";
            return str.str();
        }

}