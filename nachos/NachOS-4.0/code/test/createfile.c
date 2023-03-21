#include "syscall.h"
#define maxlen 32
int main()
{
    int fd = -1;
    char filename[256];

    PrintString("Input filename: ");
    ReadString(filename, 255);

    fd = Create(filename);
    if (fd != -1)
    {
        PrintString("\nCreate file success!!!");
    }
    else
    {
        PrintString("\nCreate file fail!!!");
    }

    Halt();
}