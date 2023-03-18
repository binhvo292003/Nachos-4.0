#include "syscall.h"
#define maxLength 32

int main(){
    char *filename;
    int openID;
    int success;

    PrintString("\nFilename to delete: ");
    ReadString(filename, maxLength);

    //Create(filename);

    openID = Open(filename, 0);
    if ( openID != -1){ // neu file ton tai, tien hanh xoa
        Close(openID); // neu file dang mo thi se dong truoc khi xoa
        success = Remove(filename);
        if (success == 0)
            PrintString("\nDelete success\n");
        else
            PrintString("\nDelete fail\n");
    }
    else
        PrintString("\nFile does not exist");
    Halt();
}