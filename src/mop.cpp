#include "lexer.cpp"
#include "parser.cpp"

using namespace std;

int main()
{
    string src = "(-2 + 3) * 2";
    auto lexer = Lexer::Tokenizer(src);
    auto parser = Parser::Ast(lexer);
    auto ast = parser.parse();
    
    auto token = lexer.next();
    
    for (auto& node : ast) {
        cout << node << endl;
    }

    return 0;
}