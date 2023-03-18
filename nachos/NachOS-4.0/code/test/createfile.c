#include "syscall.h"
#define maxlen 32
int main()
{
    int num;
    char filename[256];

    // PrintString("Input length of file: ");
    // num = ReadNum();

    PrintString("Input filename: ");
    ReadString(filename, num);

    Create(filename);

    Halt();
}