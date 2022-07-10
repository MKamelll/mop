#pragma once
#include "lexer.h"
#include "visitor.h"
#include<string>
#include<memory>

namespace astnode {

class AstNode
{
public:
    virtual std::string accept(std::unique_ptr<visitor::Visitor<std::string>> visitor) = 0;
    friend std::ostream & operator<<(std::ostream & str, AstNode & node);
};

std::ostream & operator<<(std::ostream & str, AstNode & node);

class PrimaryNode : public AstNode
{
    std::variant<std::string, int, float> mValue;
public:
    PrimaryNode(std::variant<std::string, int, float> value);
    PrimaryNode(lexer::Lexeme value);

    std::string getType();
    std::variant<std::string, int, float> getValue();
    std::string accept(std::unique_ptr<visitor::Visitor<std::string>> visitor) override;
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

    std::string accept(std::unique_ptr<visitor::Visitor<std::string>> visitor) override;
};

class PrefixNode : public AstNode
{
    std::string mOp;
    std::unique_ptr<AstNode> mRhs;

public:
    PrefixNode(std::string op, std::unique_ptr<AstNode> rhs);
    std::string getOp();
    AstNode & getRhs();

    std::string accept(std::unique_ptr<visitor::Visitor<std::string>> visitor) override;
};

}