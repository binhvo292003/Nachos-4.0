#include "syscall.h"
#define maxLength 32

int main()
{
    char source[maxLength];
    char dest[maxLength];
    int sourceFileID;
    int destFileID;
    int size;
    int i, length;
    char c;

    PrintString("1st file name: ");
    ReadString(source, maxLength);
    PrintString(source);
    PrintString("2nd file name: ");
    ReadString(dest, maxLength);
    PrintString(dest);

    sourceFileID = Open(source, 1);
    destFileID = Open(dest, 1);

    if (sourceFileID != -1 && destFileID != -1)
    {

        size = Seek(-1, destFileID);

        Seek(-1, sourceFileID); // Seek den dau file nguon
        Seek(0, destFileID);    // Seek den cuoi file dich

        i = 0;
        for (; i < size; i++)
        {
            Read(c, 1, destFileID); // Doc tung ki tu cua file nguon
            Write(c, 1, sourceFileID);
            PrintString(c);
        }

        size = Seek(-1, sourceFileID);
    }
    else
    {
        PrintString("\nFile source or dest not exist!!!");
    }

    Halt();
}