#include "syscall.h"
#define maxLength 32

int main(){
    int i;
    for(i = 0; i < 100; i++)
        PrintString("Ping");
    
    Exit(2);
}