// filesys.h
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system.
//	The "STUB" version just re-defines the Nachos file system
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.
//
//	The other version is a "real" file system, built on top of
//	a disk simulator.  The disk is simulated using the native UNIX
//	file system (in a file named "DISK").
//
//	In the "real" implementation, there are two key data structures used
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "sysdep.h"
#include "openfile.h"

//Socket
#include <sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>	//inet_addr
#include <unistd.h> //close
//---------------------------------

#ifdef FILESYS_STUB // Temporarily implement file system calls as
// calls to UNIX, until the real file system
// implementation is available
class FileSystem
{
public:
	OpenFile **openTable;
	int index;

	FileSystem()
	{
		openTable = new OpenFile *[20];
		index = 0;
		for (int i = 0; i < 20; i++)
		{
			openTable[i] = NULL;
		}
		this->Create("stdin", 0);
		this->Create("stdout", 0);
		openTable[index++] = this->Open("stdin");
		openTable[index++] = this->Open("stdout");
		openTable[0]->filename = new char[128];
		strcpy(openTable[0]->filename, "stdin");
		openTable[1]->filename = new char[128];
		strcpy(openTable[1]->filename, "stdout");
	}

	~FileSystem()
	{
		for (int i = 0; i < 20; i++)
		{
			if (openTable[i] != NULL) {
				// if (openTable[i]->filename != NULL)
				// 	{
				// 		delete openTable[i]->filename;
				// 		openTable[i]->filename = NULL;
				// 	}

				delete openTable[i];
				openTable[i] =NULL;
			}
		}
		delete[] openTable;
	}

	bool Create(char *name, int size)
	{
		int fileDescriptor = OpenForWrite(name);

		if (fileDescriptor == -1)
			return FALSE;
		Close(fileDescriptor);
		return TRUE;
	}

	OpenFile *Open(char *name)
	{
		int fileDescriptor = OpenForReadWrite(name, FALSE);

		if (fileDescriptor == -1)
			return NULL;
		return new OpenFile(fileDescriptor);
	}


	bool Remove(char *name) { return Unlink(name) == 0; }

	//Socket------------------------------------------------

	int CreateSocket(){ 
        int socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc != -1)
            return socket_desc;
        else
            return -1;
    }

    int Connect(int socket_desc,char *ip, int port){
        struct sockaddr_in server;
        
        server.sin_addr.s_addr = inet_addr(ip);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        if (ip == NULL || port < 0)
            return -1;

        if (connect(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            puts("\nconnect error");
            return -1;
        }
        puts("\nConnected");
	        return 0;
    }
	
	int Send(int socket_desc,char* message, int size){
		int state;
		state = send(socket_desc, message, size, 0);
		if (state > 0)
			return state;
		else
			return -1; 
	}

	int Receive(int socket_desc, char* buffer, int size){
		return recv(socket_desc, buffer , size , 0);
	}

};

#else // FILESYS
class FileSystem
{
public:
	OpenFile **openTable;
	int index;

	FileSystem(bool format); // Initialize the file system.
							 // Must be called *after* "synchDisk"
							 // has been initialized.
							 // If "format", there is nothing on
							 // the disk, so initialize the directory
							 // and the bitmap of free blocks.

	bool Create(char *name, int initialSize);
	// Create a file (UNIX creat)

	OpenFile *Open(char *name); // Open a file (UNIX open)

	bool Remove(char *name); // Delete a file (UNIX unlink)

	void List(); // List all the files in the file system

	void Print(); // List all the files and their contents

private:
	OpenFile *freeMapFile;	 // Bit map of free disk blocks,
							 // represented as a file
	OpenFile *directoryFile; // "Root" directory -- list of
							 // file names, represented as a file
};

#endif // FILESYS

#endif // FS_H
