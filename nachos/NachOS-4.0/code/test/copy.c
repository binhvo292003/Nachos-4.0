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

    PrintString("Source file name: ");
    ReadString(source, maxLength);
    PrintString(source);
    PrintString("Destination file name: ");
    ReadString(dest, maxLength);
    PrintString(dest);

    sourceFileID = Open(source, 1);
    destFileID = Open(dest, 1);
    if (sourceFileID != -1)
    { // neu file nguon ton tai

        destFileID = Open(dest, 1);

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
            PrintString(c);
        }
        // Write(c, length, destFileID); // Ghi vao file dich

        PrintString("\nSuccess\n");
        Close(destFileID); // Goi ham Close de dong file dich
    }
    else
    {
        PrintString("\nSource file does not exist");
    }

    Close(sourceFileID);
    PrintString("\n");
    Halt();
}