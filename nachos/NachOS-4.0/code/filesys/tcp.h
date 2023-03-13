#ifndef TCP_H
#define TCP_H

#include "copyright.h"
#include "utility.h"
#include "sysdep.h"

#ifdef TCP_STUB // Temporarily implement calls to
					// Nachos file system as calls to UNIX!
					// See definitions listed under #else
class SocketTCP
{
public:
	char *filename;
	SocketTCP(int f)
	{
		file = f;
		currentOffset = 0;
	}							 // open the file
	~SocketTCP() {  } // close the file

private:
	int file;
	int currentOffset;
};

#else // TCP
class FileHeader;

class SocketTCP
{
public:
	
private:
	FileHeader *hdr;  // Header for this file
	int seekPosition; // Current position within the file
};

#endif // TCP

#endif // TCP_H
