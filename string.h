#include <string>
#include <vector>
#include <sstream>
#include <iostream>

// class ISys_String : std::string {};

std::vector<std::string>split(std::string str, char delim) {
    std::stringstream ss(str);
    std::string mallocr;
    std::vector<std::string>stff{};
    while (getline(ss, mallocr, delim)) {
        stff.push_back(mallocr);
    }
    return stff;
}

// std::string checkString(std::string c, int pos) { /* checkString("\"hello, world\", 1, ...)") -> "hello, world!" */
//     std::stringstream go(c);
//     std::string p;
//     getline(go, p, '\"');
//     getline(go, p, '\"');
//     return p;
// }