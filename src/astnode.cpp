#include "astnode.h"

using namespace astnode;

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

std::string PrimaryNode::accept(std::unique_ptr<ToStringVisitor> visitor) {
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

std::string BinaryNode::accept(std::unique_ptr<ToStringVisitor> visitor) {
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

std::string PrefixNode::accept(std::unique_ptr<ToStringVisitor> visitor) {
    return visitor->visit(this);
}

////////////////////////////////////////////////////////////////////////////////////

std::string ToStringVisitor::visit(PrimaryNode * node) {
    std::stringstream str;
    if (node->getType() == "string") {
        str << "Identifier(" << std::get<std::string>(node->getValue()) << ")";
    } else if (node->getType() == "int") {
        str << "Int(" << std::get<int>(node->getValue()) << ")";
    } else if (node->getType() == "float") {
        str << "Float(" << std::get<float>(node->getValue()) << ")";
    }

    return str.str();
}

std::string ToStringVisitor::visit(BinaryNode * node) {
    std::stringstream str;
    str << "Binary(op: '" << node->getOp() << "', lhs: " << node->getLhs() << ", rhs: " << node->getRhs() << ")";
    return str.str();
}

std::string ToStringVisitor::visit(PrefixNode * node) {
    std::stringstream str;
    str << "Prfix(op: " << node->getOp() << ", rhs: " << node->getRhs() << ")";
    return str.str();
}