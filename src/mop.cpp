#include "lexer.h"
#include "parser.cpp"
#include "util.cpp"
#include <algorithm>
#include <string>
#include <string_view>

using namespace std;

void run(string & src) {
    auto lexer = lexer::Tokenizer(src);
    auto parser = parser::Ast(lexer);
    auto ast = parser.parse();
        
    for (auto& node : ast) {
        cout << *node << endl;
    }
}

void repl() {
    cout << "Welcome to mop.." << endl;
    while (true) {
        try {
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
        } catch (exception & e) {
            cout << e.what() << endl;
            continue;
        }
    }
}

int main()
{
    repl();
    return 0;
}