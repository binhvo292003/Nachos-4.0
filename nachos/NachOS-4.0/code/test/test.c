#include "syscall.h"
#define maxLength 32

int main()
{
    int newProc1;
    int newProc2;
    int newProc3;
    int exitCode1, exitCode2, exitCode3;

    newProc1 = Exec("ping"); // Project 01
    newProc2 = Exec("pong"); // Project 01
    newProc3 = Exec("pung");
    exitCode1 = Join(newProc1);
    exitCode2 = Join(newProc2);
    exitCode3 = Join(newProc3);
    // PrintNmber(newProc2);
    if (exitCode1 == 2) // bang voi argument trong Exit() cua ping.c
    {
        PrintString("\nThread 1 done\n");
    }
    else
        PrintString("\nThread 1 failded\n");

    if (exitCode2 == 2) // bang voi argument trong Exit() cua pong.c
    {
        PrintString("\nThread 2 done\n");
    }
    else
        PrintString("\nThread 2 failded\n");
    
    if (exitCode3 == 2) // bang voi argument trong Exit() cua pung.c
    {
        PrintString("\nThread 3 done\n");
    }
    else
        PrintString("\nThread 3 failded\n");
    Halt();
}