#include "syscall.h"

int main() {
    char* args[] = {"pung", "ping", "pong"};
    int sp = ExecV(3, args);
    Join(sp);
    Exit(0);
}