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
    PrintString("2nd file name: ");
    ReadString(dest, maxLength);

    sourceFileID = Open(source, 1);
    destFileID = Open(dest, 1);

    if (sourceFileID == -1)
    {
        PrintString("Source file does not exist\n");
        Halt();
    }

    if (destFileID == -1)
    {
        PrintString("Dest file does not exist\n");
        Halt();
    }

    size = Seek(-1, sourceFileID);

    Seek(0, sourceFileID); // Seek den dau file nguon
    Seek(-1, destFileID);    // Seek den cuoi file dich, ket hop noi dung 2 file luu vao file dich

    i = 0;
    for (; i < size; i++)
    {
        Read(c, 1, sourceFileID); // Doc tung ki tu cua file nguon
        Write(c, 1, destFileID);
    }

    PrintString("Success\n");

    Halt();
}