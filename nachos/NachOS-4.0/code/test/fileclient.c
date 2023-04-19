#include "syscall.h"
#define maxLength 32

int main(){
    char sourceFilename[maxLength];
    char destFilename[maxLength];
    int source, dest, size;
    int socket;
    char buffer[256];
    int i;

    char server_ip[] = "127.0.0.1";
    int port = 50001;

    PrintString("Input source file name: ");
    ReadString(sourceFilename, maxLength);
    PrintString("Input destination file name: ");
    ReadString(destFilename, maxLength);

    source = Open(sourceFilename, 1);
    if (source == -1)
    {
        PrintString("\nSource file does not exist");
        Halt();
    }
  
    dest = Open(destFilename, 1);
    if (dest == -1)
    {
        Create(destFilename);
        dest = Open(destFilename, 1);
    }

    size = Seek(-1, source);
    socket = SocketTCP();

    if (socket == -1)
    {
        PrintString("\nCannot create socket");
        Halt();
    }
    
    if (Connect(socket,server_ip, port) < 0)
    {
        PrintString("\nCannot connect to server");
        Halt();
    }

    i = size;
    Seek(0, source);
    Seek(0, dest);
    while (i > 0)
    {
        if (i < 255)
        {
            Read(buffer, i, source);
            Write(buffer, i, socket);
            Read(buffer, i, socket);
            Write(buffer, i, dest);
            break;
        }

        Read(buffer, 255, source);
        Write(buffer, 255, socket);
        Read(buffer, 255, socket);
        Write(buffer, 255, dest);
        i = i - 255;
    }
    
    PrintString("File transfer success\n");
    Close(source);
    Close(dest);
    Close(socket);
    
    Halt();
}