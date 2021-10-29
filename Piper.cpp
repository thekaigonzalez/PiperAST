#include <iostream>
#include <sstream>
#include "string.h"
#include "newlexer.h"
#include "conf.h"
#include "Piper.h"
/// Piper



#define reset(s) s = "";
#define errorp(s) std::cout << "error: " << s << std::endl;
#define debugp(s) std::cout << s;
#define printp(s) std::cout << "piper: " << s << std::endl;
#define stream std::stringstream

#define PIP_EOF std::string::npos

int pip_checkeof(std::string const& c, char D) {
    size_t F = PIP_EOF;
    int state = 0;
    for (int i = 0; i < c.length(); ++ i) {
        char cr = c[i];

        if (cr == D && state == 0) {
            F = 1;
        } else if (cr == D && state == 2) {
            continue;
        } else if (cr == '"' && state == 0) {
            state = 2;
        } else if (cr == '"' && state == 2)
            state = 0;
    }
    return F;
}

class CString {
    private:
        std::string sc;
    public:
        explicit CString(const std::string &c) {
            sc = c;
        }
        std::string Parse() {
            std::string s = sc;
            // std::cout << s << std::endl;
            std::string newstring;
            for (uint i = 0; i < s.length(); i++) {
                if (s[i] == '\\') {
                    switch (s[i+1]) {
                        case 'n':
                            newstring = newstring + "\n";
                            break;
                        case 't':
                            newstring = newstring + "\t";
                            break;
                        case 'r':
                            newstring = newstring + "\r";
                            break;
                        case '"':
                            newstring = newstring + "\""; /* update: add \" as escape to match other programming languages' string feature */
                            break;
                        case '(':
                            newstring = newstring + "(";
                            break;
                        
                        default: std::cout << "wrong escape sequence, '" << s[i+1] << "'" << std::endl; break;
                    }
                } else {
                    if (i != 0) {
                        if (s[i-1] != '\\') { /* not escaped */
                            newstring = newstring + s[i];
                        }
                    } else {
                        newstring = newstring + s[i];
                    }
                }
            }
            return newstring;
        }

};

std::string PrettyPrint(std::string cstring) {
    // std::cout << cstring;
    if (cstring[0] != '"' && cstring[0] != '\'') {
                std::cout << "\nerror: cstring: 1: not a string: " << cstring << std::endl;
                return "ERROR";
    }
    if (cstring[0] == '\'') {
        std::cout << "error: cstring: 1: not a string, instead character.\nDid you mean, ' \" ' ?" << std::endl;
        return "ERROR";
    }
    std::string OPEN;
    std::stringstream ss(cstring);
    std::string entstring;
    getline(ss, OPEN, '\"');
    if (cstring[cstring.length()-1] == '\'') {
        std::cout << "error: cstring: EOS: couldn't match \" with ' found at character index " << cstring.length()-1 << std::endl;
    }
    getline(ss, entstring, '\"');
    return CString(entstring).Parse();
}


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

typedef std::vector<std::string> PIP_ARRAY;

/// Returns an Action Tree for Piper code.
PiperObject *PiperCompile(std::string statement) {
    std::string key_or_funcname;

    stream stat_stream(statement);

    PiperObject *pobj = new PiperObject();

    if (split(statement,' ')[0] == PIP_VAR_KEYWORD) {
        stat_stream >> key_or_funcname;

        pobj->isfunc = false;
        pobj->key = key_or_funcname;

        stat_stream >> pobj->name;
        getline(stat_stream, pobj->val, '\n');
        pobj->val = trim(pobj->val);

    } else {
        if (pip_checkeof(statement, '(') != PIP_EOF) {
            if (pip_checkeof(statement, ')') == PIP_EOF) {
                pobj->error = 1; /* is an error */
                pobj->endofstatement = true; /* EOF, not supposed to be, but EOF. */
                pobj->isfunc = false; /* no longer a function, instead error */
                pobj->val = "null"; /* set the value to null (any expectancies) */
                pobj->msg = "<EOF> was not expected."; /* msg */
            }
            pobj->isfunc = true; /* set 'isfunc' to true because (is a function) */
            
            getline(stat_stream, pobj->funcname, '('); /* get the function name */
            
            pobj->funcargs = last_parse(stat_stream.str().substr(stat_stream.str().find("(")), ')'); /* parse up to ')' */

            pobj->funcarray = last_arg(pobj->funcargs); /* Get args */
        } else {
            pobj->error = 1; /* set error to 1 */
            pobj->key = split(statement, ' ')[0]; /* set the value to statement */
            pobj->msg = "Unknown declaration, '" + split(statement, ' ')[0] + "'"; /* set the error message */

        }
    }
    
    return pobj;
}



// int main() {
//     PiperObject *PObj = PiperCompile("decl C (\"hello\")");  
//     printp("Piper version 1.0");

//     if (PObj->isfunc) {
//         std::string name = PObj->funcname;
//         PIP_ARRAY args = PObj->funcarray;

//         std::cout << args[0] << std::endl;

//         std::cout << name << std::endl;
//     } else {
//         std::string name = PObj->name;
//         std::string value = PObj->val;

//         printp(name << " val: " << value);
//     }
    
// }