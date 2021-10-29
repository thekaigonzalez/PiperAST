#include <iostream>
#include <sstream>
#include <vector>

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

std::string last_parse(std::string last,char tok) {
    std::string ns;
    int istring = 0;
    for (int i = 0;
     i < last.length() ; ++ i) {
        if (last[i] == '"' && istring == 0) {
            // std::cout << "Beginning of string" << std::endl;
            istring = 1;
            ns = ns + last[i];
        }
        else if (last[i] == '"' && istring == 1) {
            // std::cout << "End of String, value: " << last[i] << std::endl;
            istring = 2;
            ns = ns + last[i];
        }
        else if (last[i] == tok && istring == 2) {
            ns = ns + last[i];
            // std::cout << "Signal Stop, end " << ns << std::endl; 
            istring = 3; 
            break;
        } else {
            ns = ns + last[i];
        }
        
    }
    return ns;
}
bool endsWithR(std::string const&FS, std::string const&e) {
    if (FS.length() >= e.length()) {
        return (0 == FS.compare(FS.length() - e.length(), e.length(), e));
    } else {
        return false;
    }
}
std::vector<std::string>last_arg(std::string cd, char spop = ',', char trig = '(', char trig2 = ')') {
    std::string cdd = "";
    bool once = false;
    std::vector<std::string>RETEUN{};
    int state = -1;
    // std::cout << "C: " <<  cd << std::endl;
    for (int i = 0; i < cd.length() ; i ++)
    {
        // std::cout << cd[i];;
        //  if (trim(cd).find(',') == std::string::npos && state == 0) {
        //     RETEUN.push_back(cd);
        //     return RETEUN;
        // }
        if (state == -1 && cd[i] == trig) {
            state = 0;
        }
        else if (state == 0 && cd[i] == trig) {
            // std::cout << "return code" << std::endl;
            state = 6;
        }
        else if (cd[i] == '"' && state == 0) { /* if it's a quote "string" */
        // std::cout << "Quote opening\n" << cd[i++];
            state = 1;
            cdd = cdd + cd[i];
        } else if (cd[i] == spop && state == 0) { /* if the character's the delimiter and not in a string */
            // std::cout << "Adding " << cdd << std::endl;
            RETEUN.push_back(trim(cdd));
            cdd = "";
            // state = 3;
            continue;
        } else if (cd[i] == spop && state == 1) { /* if the character's the delimiter and is in a string */
            // std::cout << "char: " << cd[i] << std::endl;
            cdd = cdd + cd[i];
        } else if (cd[i] == '"' && state == 1) {  /* if it's closing the string */ 
            cdd = cdd + '"';
            state = 0;
        } else if (cd[i] == trig2 && state == 0) {
            // std::cout << "Stopping" << std::endl;
            if (!cdd.empty()) {
                RETEUN.push_back(cdd);
            }
            if (RETEUN.size() < 1) {
                if (cdd.empty()) {
                    return {};
                }
                return {cdd};
            } else {
                return RETEUN;
            }
        } else if (state == 6 && cd[i] == trig2) {
            cdd = cdd + cd[i];
            state = 0;
        } else if (state == 6 && cd[i] == '\0') {
            std::cout << "lexer: error: <EOF> came as a surprise to me" << std::endl;    
        }// else if (cd[i] == ')' && state == 3) {
        //     RETEUN.push_back(cdd);
        //     return RETEUN;
        // }
        // } else if (cd[i] == ')' && state == 3) {
        //     state = 0;
        // }
        // } else if (cd[i] == ')' && state == 5) {
        //     RETEUN.push_back(cdd);
        //     return RETEUN;
        // }
        else {
            cdd = cdd + cd[i];
        }
            
    }
    // std::cout << RETEUN[0] << " : " << RETEUN[1] << std::endl;
    // std::cout << cdd << std::endl;
    return RETEUN;
}

/// replacement for the existing var lexer
/// var c = 1 == 1
/// value: 1
/// var c = 1 == 1
/// gets c = 1 == 1
std::vector<std::string>last_var(std::string stat) {
    std::vector<std::string>vplacement;
    
    std::string name = stat.substr(0, stat.find("="));
    // std::cout << stat.find('=');
    std::string value = stat.substr(stat.find('=')+1, stat.find('\n'));


    // std::cout << value << " and " << name << std::endl;
    vplacement.push_back(name);
    vplacement.push_back(value);

    return vplacement;
}
