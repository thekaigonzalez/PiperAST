#include "Piper.h"


int main() {
    PiperObject *p = PiperCompile("decl e 100");
    printf("%s", p->name.c_str()); // "e"
}