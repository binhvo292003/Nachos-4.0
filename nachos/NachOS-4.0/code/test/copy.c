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

    PrintString("source file name: ");
    ReadString(source, maxLength);
    PrintString("destination file name: ");
    ReadString(dest, maxLength);
   
    sourceFileID = Open(source, 1);
    destFileID = Open(dest, 1);
    if (sourceFileID != -1)
    { // neu file nguon ton tai

        if (destFileID == -1)
        {
            Create(dest);
            destFileID = Open(dest, 1);
        }

        size = Seek(-1, sourceFileID);

        Seek(0, sourceFileID); // Seek den dau file nguon
        Seek(0, destFileID);   // Seek den dau file dich

        i = 0;
        for (; i < size; i++)
        {
            Read(c, 1, sourceFileID); // Doc tung ki tu cua file nguon
            Write(c, 1, destFileID);
        }

        PrintString("\nsuccess\n");
        Close(destFileID); // Goi ham Close de dong file dich
    }
    else
    {
        PrintString("\n source file does not exist");
    }

    Close(sourceFileID);
    Halt();
}