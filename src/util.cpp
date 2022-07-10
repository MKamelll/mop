#include <string>

namespace util
{
    void trimLeft(std::string & s) {
        auto start = s.begin();
        auto end = s.end();

        for (auto it = s.begin(); it != s.end(); it++) {
            if (!isspace(*it)) {
                start = it;
                break;
            }
        }

        s = std::string(start, end);
    }

    void trimRight(std::string & s) {
        auto start = s.rbegin();
        auto end = s.rend();

        for (auto it = s.rbegin(); it != s.rend(); it++) {
            if (!isspace(*it)) {
                start = it;
                break;
            }
        }

        s = std::string(end.base(), start.base());
    }

    void trim(std::string & s) {
        trimLeft(s);
        trimRight(s);
    }

}