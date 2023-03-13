#include "syscall.h"
#define maxlen 32
int main()
{
    // int len;
    // char filename[maxlen + 1];
    // char buf[32];
    // /*Create a file*/
    // int CloseStatus;
    // int byteRead; // so byte thuc su doc duoc
    // int OpenFileID = Open("abc.txt", 0);
    // if (OpenFileID != -1)
    // {
    //     PrintString("Mo file thanh cong\n");
    //     Write("Hello", 5, OpenFileID); // Lỗi không read xong write ngay được, chỉ read hoặc write được 1 lần duy nhất trong 1 ctrinh
    //     byteRead = Read(buf, 26, OpenFileID);
    //     PrintNumber(byteRead);
    //     // PrintString(buf);
    //     Seek(3, OpenFileID);
    //     CloseStatus = Close(OpenFileID);
    //     if (CloseStatus == 0)
    //         PrintString("\nDong file thanh cong\n");
    // }
    // Halt();

    char* filename;
    int fileID;
    char temp;
    int size;

    fileID = Open("abc.txt", 0);
    size = Seek(5, fileID);

    Seek(0, fileID);
    while (size > 0)
    {
        Read(&temp, 1, fileID);
        PrintString(&temp);
        size--;
    }
    Close(fileID);
    Remove("abc1.txt");


    Halt();

}