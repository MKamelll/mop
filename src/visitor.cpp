#include "visitor.h"
#include "astnode.h"

namespace visitor {

std::string ToStringVisitor::visit(astnode::PrimaryNode * node) {
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

std::string ToStringVisitor::visit(astnode::BinaryNode * node) {
    std::stringstream str;
    str << "Binary(op: '" << node->getOp() << "', lhs: " << node->getLhs() << ", rhs: " << node->getRhs() << ")";
    return str.str();
}

std::string ToStringVisitor::visit(astnode::PrefixNode * node) {
    std::stringstream str;
    str << "Prfix(op: " << node->getOp() << ", rhs: " << node->getRhs() << ")";
    return str.str();
}

}