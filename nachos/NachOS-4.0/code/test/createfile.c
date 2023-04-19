#include "syscall.h"
#define maxlen 32
int main()
{
    int num;
    char filename[256];


    PrintString("Input filename: ");
    ReadString(filename, 255);

    if (Create(filename) < 0)
        PrintString("Create file fail\n");
    else
        PrintString("Create file success\n");

    Halt();
}