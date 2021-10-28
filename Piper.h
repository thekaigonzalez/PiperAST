#include <string>
#include <vector>
class PiperObject {
public:
    int state; /* global state */
    std::string val; /* Value of a "var" declaration */
    bool isfunc; /* is function? */
    std::string name; /* name of a variable */
    bool endofstatement; /* end of the statement */
    std::string funcname; /* function name (if function) */
    int error; /* error code */
    int func; /* function decl? */
    std::string msg; /* error msg */
    std::string key; /* keyword used (AST) */
    std::string funcargs; /* args of function (if functcion) */
    std::vector<std::string>funcarray; /* function argument array (based <funcargs>) */
};

PiperObject *PiperCompile(std::string statement);