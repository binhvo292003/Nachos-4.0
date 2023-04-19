#include "syscall.h"
#define maxLength 32

int main(){
    char server_ip[]  = "127.0.0.1";
    int port = 50000;

    int socket1 = SocketTCP();
    int socket2 = SocketTCP();
    int socket3 = SocketTCP();
    int socket4 = SocketTCP();

    char buffer[255];
    
    Connect(socket1,server_ip, port);
    Connect(socket2,server_ip, port);
    Connect(socket3,server_ip, port);
    Connect(socket4,server_ip, port);
    
    if (Write("hello socket1", 13, socket1) < 0)
        PrintString("\nsocket1 can not send message!");
    else
        PrintString("\nsocket1 sent message");
    if (Read(buffer, 13, socket1) < 0)
        PrintString("\nsocket1 can not receive message from server!");
    PrintString("\nsocket1: message from server: ");
    PrintString(buffer);

    if (Write("hello socket2", 13, socket2) < 0)
        PrintString("\nsocket2 can not send message!");
    else
        PrintString("\nsocket2 sent message");
    if (Read(buffer, 13, socket2) < 0)
        PrintString("\nsocket2 can not receive message from server!");
    PrintString("\nsocket2: message from server: ");
    PrintString(buffer);

    if (Write("hello socket3", 13, socket3) < 0)
        PrintString("\nsocket3 can not send message!");
    else
        PrintString("\nsocket3 sent message");
    if (Read(buffer, 13, socket3) < 0)
        PrintString("\nsocket3 can not receive message from server!");
    PrintString("\nsocket3: message from server: ");
    PrintString(buffer);

    if (Write("hello socket4", 13, socket4) < 0)
        PrintString("\nsocket4 can not send message!");
    else
        PrintString("\nsocket4 sent message");
    if (Read(buffer, 13, socket4) < 0)
        PrintString("\nsocket4 can not receive message from server!");
    PrintString("\nsocket4: message from server: ");
    PrintString(buffer);

    PrintString("\n");

    Close(socket1);
    Close(socket2);
    Close(socket3);
    Close(socket4);

    Halt();
}