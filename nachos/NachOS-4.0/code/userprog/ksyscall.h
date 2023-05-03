/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include <stdlib.h>



void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysExec(char* name) {
    OpenFile* oFile = kernel->fileSystem->Open(name);
    
    // Kiem tra xem file co ton tai khong
    if (oFile == NULL) {
        DEBUG(dbgSys, "\nExec:: Can't open this file.");
        return -1;
    }

    // Tra ve processID cua chuong trinh nap vao
    return kernel->pTab->ExecUpdate(name);
}

int SysJoin(int id) { return kernel->pTab->JoinUpdate(id); }

int SysExit(int id) { return kernel->pTab->ExitUpdate(id); }

int SysCreateSemaphore(char* name, int initialValue) {
    int res = kernel->semTab->Create(name, initialValue);

    if (res == -1) {
        DEBUG('a', "\nError creating semaphore");
        delete[] name;
        return -1;
    }

    return 0;
}

int SysWait(char* name) {
    int res = kernel->semTab->Wait(name);

    if (res == -1) {
        DEBUG('a', "\nSemaphore not found");
        delete[] name;
        return -1;
    }

    return 0;
}

int SysSignal(char* name) {
    int res = kernel->semTab->Signal(name);

    if (res == -1) {
        DEBUG('a', "\nSemaphore not found");
        delete[] name;
        return -1;
    }

    return 0;
}

int SysExecV(int argc, char* argv[]) {
    OpenFile* oFile = kernel->fileSystem->Open(argv[0]);
    if (oFile == NULL) {
        DEBUG(dbgSys, "\nExec: Can't open this file!!\n");
        return -1;
    }
    
    return kernel->pTab->ExecUpdate(argv[0]);
}



#endif /* ! __USERPROG_KSYSCALL_H__ */
