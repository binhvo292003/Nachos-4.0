#pragma once
#ifndef PTABLE_H
#define PTABLE_H
#include "pcb.h"
#include "bitmap.h"
class Semaphore;


#define MAX_PROCESS 10

class PTable
{
private:
    Bitmap *bm;
    PCB *pcb[MAX_PROCESS];
    int psize;      //  So luong phan tu cua mang
    Semaphore *bmsem;

public:
    PTable();
    PTable(int size);
    ~PTable();
    int ExecUpdate(char *name);
    int ExitUpdate(int ec);
    int JoinUpdate(int id);
    int GetFreeSlot();
    bool IsExist(int pid);
    void Remove(int pid);
    char *GetFileName(int id);
};

#endif