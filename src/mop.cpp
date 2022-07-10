#include "lexer.h"
#include "parser.h"
#include "util.h"
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

            util::trim(line);

            if (line.length() < 1) continue;

            if (line.rfind(":q", 0) == 0 || line.rfind(":Q", 0) == 0 || line.rfind(":quit", 0) == 0) {
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