// #include "syscall.h"
// #define maxLength 32

// // int main()
// // {
// //     char source[maxLength];
// //     int sourceFileID;
// //     int size;
// //     int i;
// //     char c;

// //     PrintString("source file name: ");
// //     ReadString(source, maxLength);
// //     PrintString("\n----------------------\n");
// //     PrintString(source);

// //     PrintString("\n----------------------\n");
  
// //     Create(source);

// //     sourceFileID = Open(source, 1);
// //     if (sourceFileID != -1)
// //     {
// //         // neu file nguon ton tai
// //         size = Seek(-1, sourceFileID);

// //         Seek(0, sourceFileID); // Seek den dau file nguon

// //         i = 0;
// //         for (; i < size; i++)
// //         {
// //             Read(c, 1, sourceFileID); // Doc tung ki tu cua file nguon
// //             PrintString(c);
// //         }
// //         PrintString("\nsuccess\n");
// //     }

// //     else
// //     {
// //         PrintString("\n source file does not exist");
// //     }
// //     Close(sourceFileID);
// //     Halt();
// // }

// int main(){
//     //int socket = SocketTCP();

//     // int socket1 = SocketTCP();
//     // int socket2 = SocketTCP();
//     // int socket3 = SocketTCP();
//     // int socket4 = SocketTCP();

//     // int sentsize;
//     // int receivesize;
//     // char buffer[27];

//     // if (socket1 != -1)
//     //     PrintString("\nsuccess1");
//     // else
//     //     PrintString("\nfail1");

//     // if (socket2 != -1)
//     //     PrintString("\nsuccess1");
//     // else
//     //     PrintString("\nfail1");
    
//     // if (socket3 != -1)
//     //     PrintString("\nsuccess1");
//     // else
//     //     PrintString("\nfail1");
    
//     // if (socket4 != -1)
//     //     PrintString("\nsuccess1");
//     // else
//     //     PrintString("\nfail1");
    

//     // if (socket != -1)
//     //     PrintString("\nsuccess1");
//     // else
//     //     PrintString("\nfail1");
    
//     // if (Connect(socket,"127.0.0.1", 50000) == 0)
//     //     PrintString("\nsuccess2");
//     // else
//     //     PrintString("\nfail2");

//     // sentsize = Write("hello from the other side", 27, socket);
//     // if (sentsize > 0)
//     // {
//     //     PrintString("\nsuccess3");
//     //     PrintNumber(sentsize);
//     // }
//     // else
//     //     PrintString("\nfail3");

//     // receivesize = Read(buffer, 27, socket);
//     // if (receivesize > 0)
//     // {
//     //     PrintString("\nsuccess4\n");
//     //     PrintNumber(sentsize);
//     //     PrintString(buffer);
//     // }
//     // else
//     //     PrintString("\nfail4");

//     // Close(socket);

    
//     // Connect(socket1,"127.0.0.1", 50000);
    
//     // Connect(socket2,"127.0.0.1", 50000);
    
//     // Connect(socket3,"127.0.0.1", 50000);
    
//     // Connect(socket4,"127.0.0.1", 50000);
    



//     // Write("hello from the other side 1", 27, socket1);
//     // Read(buffer, 27, socket1);
//     // PrintString(buffer);



//     // Write("hello from the other side 2", 27, socket2);
//     // Read(buffer, 27, socket2);
//     // PrintString(buffer);



//     // Write("hello from the other side 3", 27, socket3);
//     // Read(buffer, 27, socket3);
//     // PrintString(buffer);


//     // Write("hello from the other side 4", 27, socket4);
//     // Read(buffer, 27, socket4);
//     // PrintString(buffer);


//     // Close(socket1);
//     // Close(socket2);
//     // Close(socket3);
//     // Close(socket4);

//     // char filename[256];

//     // PrintString("\n hello : ");
    
//     // //int num;
//     // ReadString(filename, 250);
//     // PrintString(filename);
//     // PrintString("\nhaha");



//     int num1 = 10;
   
//         if (num1 == 10){
//             PrintString("\n if num  == 10");
//             Halt();}
//         PrintString("\nthe num is not 10");
    

//     PrintString("\nend program");





//     Halt();
// }

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