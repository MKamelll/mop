#include "lexer.cpp"
#include<vector>
#include<memory>

namespace Parser
{
    using namespace std;
    
    class ParserError : public exception
    {
        public:
        const char * mMesg;
        ParserError(const char * mesg) : mMesg(mesg) {}
        
        virtual const char* what() const throw()
        {
            return mMesg;
        }
    };

    class AstNode {
    protected:
        virtual void print(ostream & str) = 0;
    public:
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
            } else if (holds_alternative<string>(mValue)) {
                return "string";
            }
        }
    };

    class NumberNode : public PrimaryNode {
        using PrimaryNode::PrimaryNode;
    };
    class IdentifierNode : public PrimaryNode {
        using PrimaryNode::PrimaryNode;
    };

    enum class Assoc
    {
        RIGHT, LEFT
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

        Lexer::Token previous() {
            return mPrevToken;
        }

        bool check(Lexer::TokenType type) {
            if (type == mCurrToken.getType()) {
                return true;
            }

            return false;
        } 

        bool match(vector<Lexer::TokenType> types) {
            for (auto& type : types) {
                if (check(type)) {
                    advance();
                    return true;
                }
            }

            return false;
        }

    public:
        Ast(Lexer::Tokenizer lexer) : mLexer(lexer), mCurrToken(mLexer.next()), mPrevToken(mCurrToken) {}
        
        vector<unique_ptr<AstNode>> parse() {
            if (isAtEnd()) return move(mSubTrees);

            unique_ptr<AstNode> expr = parseExpr();

            mSubTrees.push_back(move(expr));

            return parse();
        }

        unique_ptr<AstNode> parseExpr() {
            unique_ptr<AstNode> lhs = parsePrimary();
                        
            mRoot = move(lhs);
            return move(mRoot);           
        }

        unique_ptr<AstNode> parsePrimary() {
            return parseNumber();
        }

        unique_ptr<AstNode> parseNumber() {
            if (match({Lexer::TokenType::INT, Lexer::TokenType::FLOAT})) {
                auto node = make_unique<NumberNode>(NumberNode(previous().getLexeme()));
                return node;
            }

            return parseIdentifier();
        }

        unique_ptr<AstNode> parseIdentifier() {
            if (match({Lexer::TokenType::IDENTIFIER})) {
                auto node = make_unique<IdentifierNode>(IdentifierNode(previous().getLexeme()));
                return node;
            }

            throw ParserError("Expected a primary");
        }
    };
}