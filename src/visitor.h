#pragma once
#include<string>

namespace astnode {
    class PrimaryNode;
    class BinaryNode;
    class PrefixNode;
}

namespace visitor {

class ToStringVisitor
{
public:
    std::string visit(astnode::PrimaryNode * node);
    std::string visit(astnode::BinaryNode * node);
    std::string visit(astnode::PrefixNode * node);
};

}