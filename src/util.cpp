#pragma once
#include <string>

namespace Util
{

    using namespace std;

    void trimLeft(string & s) {
        auto start = s.begin();
        auto end = s.end();

        for (auto it = s.begin(); it != s.end(); it++) {
            if (!isspace(*it)) {
                start = it;
                break;
            }
        }

        s = string(start, end);
    }

    void trimRight(string & s) {
        auto start = s.rbegin();
        auto end = s.rend();

        for (auto it = s.rbegin(); it != s.rend(); it++) {
            if (!isspace(*it)) {
                start = it;
                break;
            }
        }

        s = string(end.base(), start.base());
    }

    void trim(string & s) {
        trimLeft(s);
        trimRight(s);
    }

}