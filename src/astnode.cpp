#include "astnode.h"

namespace astnode {
    
std::ostream & operator<<(std::ostream & str, AstNode & node) {
    str << node.accept(std::make_unique<visitor::ToStringVisitor>(visitor::ToStringVisitor()));
    return str;
}

/////////////////////////////////////////////////////////////////////////////////////

PrimaryNode::PrimaryNode(std::variant<std::string, int, float> value) : mValue(value) {}
PrimaryNode::PrimaryNode(lexer::Lexeme value) : mValue(value.getLexeme()) {}

std::string PrimaryNode::getType() {
    if (std::holds_alternative<int>(mValue)) {
        return "int";
    } else if (std::holds_alternative<float>(mValue)) {
        return "float";
    }
    return "string";
}

std::variant<std::string, int, float> PrimaryNode::getValue() {
    return mValue;
}

std::string PrimaryNode::accept(std::unique_ptr<visitor::Visitor<std::string>> visitor) {
    return visitor->visit(this);
}

/////////////////////////////////////////////////////////////////////////////

BinaryNode::BinaryNode(std::string op, std::unique_ptr<AstNode> lhs, std::unique_ptr<AstNode> rhs) 
    : mOp(op), mLhs(move(lhs)), mRhs(move(rhs)) {}

std::string BinaryNode::getOp() {
    return mOp;
}

AstNode & BinaryNode::getLhs() {
    return *mLhs;
}

AstNode & BinaryNode::getRhs() {
    return *mRhs;
}

std::string BinaryNode::accept(std::unique_ptr<visitor::Visitor<std::string>> visitor) {
    return visitor->visit(this);
}

////////////////////////////////////////////////////////////////////////////////////

PrefixNode::PrefixNode(std::string op, std::unique_ptr<AstNode> rhs) : mOp(op), mRhs(move(rhs)) {}

std::string PrefixNode::getOp() {
    return mOp;
}

AstNode & PrefixNode::getRhs() {
    return *mRhs;
}

std::string PrefixNode::accept(std::unique_ptr<visitor::Visitor<std::string>> visitor) {
    return visitor->visit(this);
}

}
