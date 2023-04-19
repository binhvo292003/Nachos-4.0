#include "syscall.h"
#define maxLength 32

int main(){
    char *filename;
    int openID;
    int success;

    PrintString("Filename to delete: ");
    ReadString(filename, maxLength);

    openID = Open(filename, 0);
    if ( openID != -1){ // neu file ton tai, tien hanh xoa
        Close(openID); // neu file dang mo thi se dong truoc khi xoa
        success = Remove(filename);
        if (success == 0)
            PrintString("Delete success\n");
        else
            PrintString("Delete fail\n");
    }
    else
        PrintString("File does not exist\n");
    Halt();
}