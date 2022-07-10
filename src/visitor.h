#pragma once
#include<string>
#include<memory>

namespace astnode {
    class PrimaryNode;
    class BinaryNode;
    class PrefixNode;
}

namespace visitor {

template<typename T>
class Visitor
{
public:
    virtual T visit(astnode::PrimaryNode * node) = 0;
    virtual T visit(astnode::BinaryNode * node) = 0;
    virtual T visit(astnode::PrefixNode * node) = 0;
};

class ToStringVisitor : public Visitor<std::string>
{
public:
    std::string visit(astnode::PrimaryNode * node) override;
    std::string visit(astnode::BinaryNode * node) override;
    std::string visit(astnode::PrefixNode * node) override;
};

}