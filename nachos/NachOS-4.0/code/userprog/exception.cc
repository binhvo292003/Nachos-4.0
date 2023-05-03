// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"

#define MaxFileLength 32
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

char *User2System(int virtAddr, int limit)
{
	int i; // index
	int oneChar;
	char *kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; // need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0, limit + 1);
	// printf("\n Filename u2s:");
	for (i = 0; i < limit; i++)
	{
		kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		// printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

int System2User(int virtAddr, int len, char *buffer)
{
	if (len < 0)
		return -1;
	if (len == 0)
		return len;
	int i = 0;
	int oneChar = 0;
	do
	{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

void ProgramCounter()
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

void SlovingSC_Halt()
{
	DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

	SysHalt();

	ASSERTNOTREACHED();
	return;
}

void SlovingSC_Add()
{
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
					/* int op2 */ (int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);

	/* Modify return point */
	{
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
	}

	return;

	ASSERTNOTREACHED();

	return;
}

void SlovingSC_Create()
{
	int virtAddr;
	char *filename;
	// Lấy tham số tên tập tin từ thanh ghi r4
	virtAddr = kernel->machine->ReadRegister(4);
	// MaxFileLength là = 32
	filename = User2System(virtAddr, MaxFileLength + 1);
	if (filename == NULL)
	{
		printf("\n Not enough memory in system");
		DEBUG('a', "\n Not enough memory in system");
		kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương
		// trình người dùng
		delete filename;
		return;
	}
	DEBUG('a', "\n Finish reading filename.");
	if (!kernel->fileSystem->Create(filename, 0))
	{
		printf("\n Error create file '%s'", filename);
		kernel->machine->WriteRegister(2, -1);
		delete filename;
		return;
	}
	kernel->machine->WriteRegister(2, 0); // trả về cho chương trình
	// người dùng thành công
	delete filename;
	ProgramCounter();
	return;
}

void SlovingSC_Open()
{
	// OpenFileId Open(char *name);
	// arg1: dia chi chuoi name

	int virtAddr = kernel->machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
	int type = kernel->machine->ReadRegister(5);	 // Lay tham so type tu thanh ghi so 5
	char *filename = User2System(virtAddr, MaxFileLength);
	int fileId = kernel->Open(filename, type);

	if (fileId == -1)
	{
		kernel->machine->WriteRegister(2, -1);
	}
	else
	{
		kernel->machine->WriteRegister(2, fileId);
	}
	delete[] filename;

	ProgramCounter();
	return;
}

void SlovingSC_Close()
{
	// Input id cua file(OpenFileID)
	// Output: 0: thanh cong, -1: that bai

	int id = kernel->machine->ReadRegister(4);

	int result = kernel->Close(id);

	if (result == 0)
	{
		kernel->machine->WriteRegister(2, 0);
	}
	else
	{
		kernel->machine->WriteRegister(2, -1);
	}

	ProgramCounter();
	return;
}

void SlovingSC_Read()
{
	//	int Read(char *buffer, int size, OpenFileId id);
	int virtAddr = kernel->machine->ReadRegister(4);
	int charCount = kernel->machine->ReadRegister(5);
	int id = kernel->machine->ReadRegister(6);
	char *buf = User2System(virtAddr, charCount);

	if (id < 0 || id > 19)
	{
		printf("\nId Error.\n");
		kernel->machine->WriteRegister(2, -1);
		ProgramCounter();
		return;
	}
	// check file/socket exist
	if (kernel->fileSystem->openTable[id] == NULL)
	{
		printf("\nFile/Socket not exist");
		kernel->machine->WriteRegister(2, -1);
		ProgramCounter();
		return;
	}
	if (kernel->fileSystem->openTable[id]->isSocket != 1) // file
	{
		int byteRead = kernel->Read(buf, charCount, id);
		System2User(virtAddr, byteRead, buf);
	}
	else // socket
	{
		int socket_desc = kernel->fileSystem->openTable[id]->socket_desc;
		int byteReceive = kernel->fileSystem->Receive(socket_desc, buf, charCount);
		if (byteReceive > 0)
		{
			//cout << "byte recv: " << byteReceive;
			kernel->machine->WriteRegister(2, byteReceive);
			System2User(virtAddr, byteReceive, buf);
		}
		else if  (byteReceive == 0)
			kernel->machine->WriteRegister(2, 0);
		else
			kernel->machine->WriteRegister(2, -1);
	}

	delete[] buf;
	ProgramCounter();
	return;
}

void SlovingSC_Write()
{
	// int Write(char *buffer, int size, OpenFileId id);
	int virtAddr = kernel->machine->ReadRegister(4);
	int charCount = kernel->machine->ReadRegister(5);
	int id = kernel->machine->ReadRegister(6);
	char *buf = User2System(virtAddr, charCount);

	if (id < 0 || id > 19)
	{
		printf("\nId Error.\n");
		kernel->machine->WriteRegister(2, -1);
		ProgramCounter();
		return;
	}
	// check file/socket exist
	if (kernel->fileSystem->openTable[id] == NULL)
	{
		printf("\nFile/Socket not exist");
		kernel->machine->WriteRegister(2, -1);
		ProgramCounter();
		return;
	}

	if (kernel->fileSystem->openTable[id]->isSocket != 1) // file
	{   
		// check file is only read
		if (kernel->fileSystem->openTable[id]->type == 0)
		{
			printf("\nFile only read");
			kernel->machine->WriteRegister(2, -1);
			ProgramCounter();
			return;
		}

		int byteWrite = kernel->fileSystem->openTable[id]->Write(buf, charCount);

		System2User(virtAddr, byteWrite, buf);
		kernel->machine->WriteRegister(2, byteWrite);
	}
	else //socket
	{
		int socket_desc = kernel->fileSystem->openTable[id]->socket_desc;
		if (kernel->fileSystem->openTable[id]->isConnected == 1)
		{
			int byteSend = kernel->fileSystem->Send(socket_desc, buf, charCount);
			if (byteSend > 0)
				kernel->machine->WriteRegister(2, byteSend);
			else
				kernel->machine->WriteRegister(2, -1);
		}
		else
			kernel->machine->WriteRegister(2, 0);
	}

	delete[] buf;
	ProgramCounter();
	return;
}

void SlovingSC_PrintString()
{
	int addr = kernel->machine->ReadRegister(4);
	char *buffer = User2System(addr, 200);
	kernel->PrintBuffer(buffer, 200);

	// cout << endl;
	delete[] buffer;
	ProgramCounter();
	return;
}

void SlovingSC_PrintNumber()
{
	int num = kernel->machine->ReadRegister(4);
	kernel->PrintNumber(num);
	ProgramCounter();
	return;
}

void SlovingSC_Seek()
{
	// Seek(int pos, OpenFileID id)
	int pos = kernel->machine->ReadRegister(4);
	int id = kernel->machine->ReadRegister(5);

	int result = kernel->Seek(pos, id);
	kernel->machine->WriteRegister(2, result);

	ProgramCounter();
	return;
}

void SlovingSC_Remove()
{
	int virtArr = kernel->machine->ReadRegister(4);
	char *filename = User2System(virtArr, MaxFileLength + 1);
	if (filename == NULL)
	{
		printf("\nNot enough memory in system\n");
		kernel->machine->WriteRegister(2, -1);
		// return -1 to user program

		ProgramCounter();
		return;
	}
	bool success = kernel->fileSystem->Remove(filename);
	if (!success)
	{
		// Fail to remove file
		printf("\nCan't remove file '%s'\n", filename);
		kernel->machine->WriteRegister(2, -1);
		// return -1 to user program

		delete filename;
		ProgramCounter();
		return;
	}

	// Success to remove file
	kernel->machine->WriteRegister(2, 0); // return 0 to user program
	cout << "\nFile removed " << filename;

	delete filename;
	ProgramCounter();

	return;
}

void SlovingSC_ReadNum()
{
	int addr = kernel->machine->ReadRegister(4);
	char *buffer = User2System(addr, 200);
	kernel->ReadNum(2);
	delete[] buffer;
	ProgramCounter();
	return;
}

#define MAX_FILE_STRING_LENGTH 255
void SlovingSC_ReadString()
{
	int memBuffer = kernel->machine->ReadRegister(4);
	int length = kernel->machine->ReadRegister(5);

	if (length > MAX_FILE_STRING_LENGTH)
	{
		SysHalt();
	}

	char *buffer = kernel->ReadString(2);
	// truyen vao length
	System2User(memBuffer, MAX_FILE_STRING_LENGTH, buffer);

	delete[] buffer;

	ProgramCounter();
	return;
}

void SlovingSC_SocketTCP()
{
	int freeSlot = -1;
	for (int i = 2; i < 20; i++)
	{
		if(kernel->fileSystem->openTable[i] == NULL)
		{
			freeSlot = i;
			break;
		}
	}
	if(freeSlot == -1)
	{
		cout << "\nopenTable is full\n";
		kernel->machine->WriteRegister(2, -1);
	}
	else
	{
		//cout << "\n creating socket" << endl;
		kernel->fileSystem->openTable[freeSlot] = new OpenFile(-1);
		kernel->fileSystem->openTable[freeSlot]->isSocket = 1;
		kernel->fileSystem->openTable[freeSlot]->socket_desc = kernel->fileSystem->CreateSocket();
		if (kernel->fileSystem->openTable[freeSlot]->socket_desc != -1)
			kernel->machine->WriteRegister(2, freeSlot);
		else
		{
			kernel->machine->WriteRegister(2, -1);
			delete kernel->fileSystem->openTable[freeSlot];
		}

	}

	ProgramCounter();
	return;
}

#define MAX_IP_LENGTH 46 // khum biet nua, tra google
void SlovingSC_Connect()
{
	int socketId = kernel->machine->ReadRegister(4);
	int ip = kernel->machine->ReadRegister(5);
	int port = kernel->machine->ReadRegister(6);
	char *severIp = User2System(ip, MAX_IP_LENGTH);

	if (socketId < 0 || socketId > 19 || socketId == 1 || socketId == 0) // socketId nam ngoai pham vi bang hoac la stdin, stdout
		kernel->machine->WriteRegister(2, -1);
	else if (kernel->fileSystem->openTable[socketId] == NULL || kernel->fileSystem->openTable[socketId]->isSocket != 1) 
		kernel->machine->WriteRegister(2, -1); // khong ton tai hoac khong phai socket
	else 
	{
		int socket_desc = kernel->fileSystem->openTable[socketId]->socket_desc;
		int state = kernel->fileSystem->Connect(socket_desc, severIp, port);
		if (state == 0)
		{
			kernel->fileSystem->openTable[socketId]->isConnected = 1;
			kernel->machine->WriteRegister(2, 0);
		}
		else
			kernel->machine->WriteRegister(2, -1);
	}
	
	delete[] severIp;
	ProgramCounter();
	return;
}

void SlovingSC_Exec() {
    int virtAddr;
	// Doc dia chi tu cua file thuc thi tu thanh ghi r4
    virtAddr = kernel->machine->ReadRegister(4);  
    char* name;
	// Lay ten chuong trinh, nap vao system de xu li
    name = User2System(virtAddr,32);  
	// Kiem tra bo nho cua he thong co du dung luong khong 
    if (name == NULL) {
        DEBUG(dbgSys, "\n Not enough memory in System");
        ASSERT(false);
        kernel->machine->WriteRegister(2, -1);
        return ProgramCounter();
    }

	// nap ten dia chi vao kernel de xu li
	int id = SysExec(name);
	//cout<<id<<endl;
    kernel->machine->WriteRegister(2, id);

    return ProgramCounter();
}


void SlovingSC_Join() {
    int id = kernel->machine->ReadRegister(4);
    kernel->machine->WriteRegister(2, SysJoin(id));
    return ProgramCounter();
}


void SlovingSC_Exit() {
    int id = kernel->machine->ReadRegister(4);
    kernel->machine->WriteRegister(2, SysExit(id));
    return ProgramCounter();
}

void SlovingSC_CreateSemaphore() {
    int virtAddr = kernel->machine->ReadRegister(4);
    int semval = kernel->machine->ReadRegister(5);

    char* name = User2System(virtAddr,32);
    if (name == NULL) {
        DEBUG(dbgSys, "\n Not enough memory in System");
        ASSERT(false);
        kernel->machine->WriteRegister(2, -1);
        delete[] name;
        return ProgramCounter();
    }

    kernel->machine->WriteRegister(2, SysCreateSemaphore(name, semval));
    delete[] name;
    return ProgramCounter();
}

void SlovingSC_Wait() {
    int virtAddr = kernel->machine->ReadRegister(4);

    char* name = User2System(virtAddr,32);
    if (name == NULL) {
        DEBUG(dbgSys, "\n Not enough memory in System");
        ASSERT(false);
        kernel->machine->WriteRegister(2, -1);
        delete[] name;
        return ProgramCounter();
    }

    kernel->machine->WriteRegister(2, SysWait(name));
    delete[] name;
    return ProgramCounter();
}

void SlovingSC_Signal() {
    int virtAddr = kernel->machine->ReadRegister(4);

    char* name = User2System(virtAddr,32);
    if (name == NULL) {
        DEBUG(dbgSys, "\n Not enough memory in System");
        ASSERT(false);
        kernel->machine->WriteRegister(2, -1);
        delete[] name;
        return ProgramCounter();
    }

    kernel->machine->WriteRegister(2, SysSignal(name));
    delete[] name;
    return ProgramCounter();
}


void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case PageFaultException:
	{
		cerr << "No valid translation found\n";
		SysHalt();
		break;
	}
	case ReadOnlyException:
	{	
		cerr << "Write attempt to read-only page detected\n";
		SysHalt();
		break;
	}
	case BusErrorException:
	{
		cerr << "Invalid physical address after translation\n";
		SysHalt();
		break;
	}
	case AddressErrorException:
	{ 
		cerr << "Invalid address space"
			 << "\n";
		SysHalt();
		break;
	}
	case OverflowException:
	{
		cerr << "Arithmetic operators resulted in integer overflow\n";
		SysHalt();
		break;
	}
	case IllegalInstrException:
	{
		cerr << "Instruction not defined\n";
		SysHalt();
		break;
	}
	case NoException:
	{
		return;
		break;
	}
	case SyscallException:
		switch (type)
		{
		case SC_Halt:
		{
			return SlovingSC_Halt();
			break;
		}

		case SC_Add:
		{
			return SlovingSC_Add();
			break;
		}

		case SC_Create:
		{
			return SlovingSC_Create();
			break;
		}

		case SC_Open:
		{
			return SlovingSC_Open();
			break;
		}

		case SC_Close:
		{
			return SlovingSC_Close();
			break;
		}

		case SC_Read:
		{
			return SlovingSC_Read();
			break;
		}

		case SC_Write:
		{
			return SlovingSC_Write();
			break;
		}

		case SC_PrintString:
		{
			return SlovingSC_PrintString();
			break;
		}

		case SC_PrintNumber:
		{
			return SlovingSC_PrintNumber();
			break;
		}

		case SC_Seek:
		{
			return SlovingSC_Seek();
			break;
		}

		case SC_Remove:
		{
			return SlovingSC_Remove();
			break;
		}

		case SC_SocketTCP:
		{
			return SlovingSC_SocketTCP();
			break;
		}

		case SC_Connect:
		{
			return SlovingSC_Connect();
			break;
		}

		case SC_ReadString:
		{
			return SlovingSC_ReadString();
			break;
		}

		case SC_ReadNum:
		{
			return SlovingSC_ReadNum();
			break;
		}

		case SC_Exec:
		{
			return SlovingSC_Exec();
			break;
		}

		case SC_Join:
		{
			return SlovingSC_Join();
			break;
		}

		case SC_Exit:
		{
			return SlovingSC_Exit();
			break;
		}

		case SC_CreateSemaphore:
		{
			return SlovingSC_CreateSemaphore();
			break;
		}

		case SC_Wait:
		{
			return SlovingSC_Wait();
			break;
		}

		case SC_Signal:
		{
			return SlovingSC_Signal();
			break;
		}

		default:
			cerr << "Unexpected system callinggggggggggggg " << type << "\n";
			break;
		}
		return;
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}
