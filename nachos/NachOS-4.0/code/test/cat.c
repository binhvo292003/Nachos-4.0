#include "syscall.h"
#define maxLength 32

int main()
{
    char source[maxLength];
    int sourceFileID;
    int size;
    int i;
    char c;

    PrintString("source file name: ");
    ReadString(source, maxLength);
    PrintString(source);

    sourceFileID = Open(source, 1);
    if (sourceFileID != -1)
    {
        // neu file nguon ton tai
        size = Seek(-1, sourceFileID);

        Seek(0, sourceFileID); // Seek den dau file nguon

        PrintString("\nContent:\n");
        i = 0;
        for (; i < size; i++)
        {
            Read(c, 1, sourceFileID); // Doc tung ki tu cua file nguon
            PrintString(c);
        }

    }

    else
    {
        PrintString("\n source file does not exist");
    }
    Close(sourceFileID);
    Halt();
}