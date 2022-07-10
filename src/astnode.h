#pragma once
#include "lexer.h"

namespace astnode {

class PrimaryNode;
class BinaryNode;
class PrefixNode;

class ToStringVisitor
{
public:
    std::string visit(PrimaryNode * node);
    std::string visit(BinaryNode * node);
    std::string visit(PrefixNode * node);
};

class AstNode {
public:
    virtual std::string accept(std::unique_ptr<ToStringVisitor> visitor) = 0;
    friend std::ostream & operator<<(std::ostream & str, AstNode & node) {
        str << node.accept(std::make_unique<ToStringVisitor>(ToStringVisitor()));
        return str;
    }
};

class PrimaryNode : public AstNode
{
    std::variant<std::string, int, float> mValue;
public:
    PrimaryNode(std::variant<std::string, int, float> value);
    PrimaryNode(lexer::Lexeme value);

    std::string getType();
    std::variant<std::string, int, float> getValue();
    std::string accept(std::unique_ptr<ToStringVisitor> visitor) override;
};

class NumberNode : public PrimaryNode {
    using PrimaryNode::PrimaryNode;
};
class IdentifierNode : public PrimaryNode {
    using PrimaryNode::PrimaryNode;
};

class BinaryNode : public AstNode
{
    std::string mOp;
    std::unique_ptr<AstNode> mLhs;
    std::unique_ptr<AstNode> mRhs;

public:
    BinaryNode(std::string op, std::unique_ptr<AstNode> lhs, std::unique_ptr<AstNode> rhs);
    std::string getOp();
    AstNode & getLhs();
    AstNode & getRhs();

    std::string accept(std::unique_ptr<ToStringVisitor> visitor) override;
};

class PrefixNode : public AstNode
{
    std::string mOp;
    std::unique_ptr<AstNode> mRhs;

public:
    PrefixNode(std::string op, std::unique_ptr<AstNode> rhs);
    std::string getOp();
    AstNode & getRhs();

    std::string accept(std::unique_ptr<ToStringVisitor> visitor) override;
};

}