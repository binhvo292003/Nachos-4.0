#include "syscall.h"
#define maxLength 32

int main(int argc, char* argv[]){
    int i;
    for(i = 0; i < 100; i++)
        PrintString("pung");
    Exit(2);
}