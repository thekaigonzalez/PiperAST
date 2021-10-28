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
        stat_stream >> pobj->val;

    } else {
        if (statement.find("(") != PIP_EOF) {
            if (statement.find(")") == PIP_EOF) {
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
            pobj->val = statement; /* set the value to statement */
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