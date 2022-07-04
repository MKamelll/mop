#include "lexer.cpp"

using namespace std;

int main()
{
    string src = "( + - * / ) 12.52 v c x %";
    auto lexer = Lexer::Tokenizer(src);

    auto token = lexer.next();
    while (token.getType() != Lexer::TokenType::ENDOFFILE) {
        cout << token << endl;
        token = lexer.next();
    }
    
    cout << token << endl;
    
    return 0;
}