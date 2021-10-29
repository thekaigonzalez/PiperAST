#include "Piper.h"


int main() {
    PiperObject *p = PiperCompile("print(\"hello)\"");

    printf("%s", p->msg.c_str());
    printf("%s", p->name.c_str()); // "e"
}