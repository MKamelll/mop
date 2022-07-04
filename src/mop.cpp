#include "parser.cpp"

using namespace std;

int main()
{
    string src = "12 13 20 15.32 v c x";
    auto lexer = Lexer::Tokenizer(src);
    auto parser = Parser::Ast(lexer);
    auto ast = parser.parse();
    
    for (auto& node : ast) {
        cout << node << endl;
    }
    
    return 0;
}