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
	DEBUG('a', "\n SC_Create call ...");
	DEBUG('a', "\n Reading virtual address of filename");
	// Lấy tham số tên tập tin từ thanh ghi r4
	virtAddr = kernel->machine->ReadRegister(4);
	DEBUG('a', "\n Reading filename.");
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
	// DEBUG('a',"\n File name : '"<<filename<<"'");
	//  Create file with size = 0
	//  Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
	//  việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
	//  hành Linux, chúng ta không quản ly trực tiếp các block trên
	//  đĩa cứng cấp phát cho file, việc quản ly các block của file
	//  trên ổ đĩa là một đồ án khác
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

	return;
}

void SlovingSC_Open()
{
	// OpenFileId Open(char *name);
	// arg1: dia chi chuoi name

	int virtAddr = kernel->machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
	int type = kernel->machine->ReadRegister(5);	 // Lay tham so type tu thanh ghi so 5
	char *filename = User2System(virtAddr, MaxFileLength);
	int fileId = kernel->Open(filename);

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

	cout<<endl;
	for (int i = 0; i < 20; i++)
	{
		if (kernel->fileSystem->openTable[i] != NULL)
		{
			cout << "OpenFileID: " << i << "\tName: " << kernel->fileSystem->openTable[i]->filename << endl;
		}
	}

	ProgramCounter();
	return;
}

void SlovingSC_Read()
{
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
	// check file exist
	if (kernel->fileSystem->openTable[id] == NULL)
	{
		printf("\nFile not exist");
		kernel->machine->WriteRegister(2, -1);
		ProgramCounter();
		return;
	}

	int byteRead = kernel->Read(buf, charCount, id);

	System2User(virtAddr, byteRead, buf);
	delete[] buf;
	ProgramCounter();
	return;
}

void SlovingSC_Write()
{
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
	// check file exist
	if (kernel->fileSystem->openTable[id] == NULL)
	{
		printf("\nFile not exist");
		kernel->machine->WriteRegister(2, -1);
		ProgramCounter();
		return;
	}

	int byteWrite = kernel->fileSystem->openTable[id]->Write(buf, charCount);

	System2User(virtAddr, byteWrite, buf);
	kernel->machine->WriteRegister(2, byteWrite);
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
	printf("\nFile removed '%s'\n", filename);

	delete filename;
	ProgramCounter();

	return;
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			return SlovingSC_Halt();

		case SC_Add:
			return SlovingSC_Add();

		case SC_Create:
			return SlovingSC_Create();

		case SC_Open:
			return SlovingSC_Open();

		case SC_Close:
			return SlovingSC_Close();

		case SC_Read:
			return SlovingSC_Read();

		case SC_Write:
			return SlovingSC_Write();

		case SC_PrintString:
			return SlovingSC_PrintString();

		case SC_PrintNumber:
			return SlovingSC_PrintNumber();

		case SC_Seek:
			return SlovingSC_Seek();

		case SC_Remove:
			return SlovingSC_Remove();

		default:
			cerr << "Unexpected system callinggggggggggggg " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	// ASSERTNOTREACHED();
}
