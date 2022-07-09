#include "lexer.cpp"
#include "parser.cpp"
#include "util.cpp"
#include <algorithm>
#include <string>
#include <string_view>

using namespace std;

void run(string & src) {
    auto lexer = Lexer::Tokenizer(src);
    auto parser = Parser::Ast(lexer);
    auto ast = parser.parse();
    
    auto token = lexer.next();
    
    for (auto& node : ast) {
        cout << node << endl;
    }
}

void repl() {
    cout << "Welcome to mop.." << endl;
    while (true) {
        cout << ">> ";
        string line;
        getline(cin, line);

        Util::trim(line);

        if (line.length() < 1) continue;

        auto linesv = string_view(line);
        if (linesv.find_first_of(":q") == 0 || linesv.find_first_of(":Q") == 0 || linesv.find_first_of(":quit") == 0) {
            cout << "Goodbye!" << endl;
            exit(0);
        }

        run(line);
    }
}

int main()
{
    try {
        repl();
    } catch (exception & e) {
        cout << e.what() << endl;
    }

    return 0;
}