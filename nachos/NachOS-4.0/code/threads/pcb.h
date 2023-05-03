#ifndef PCB_H
#define PCB_H
#pragma once
#include "thread.h"
class Semaphore;

class PCB {
   private:
    Semaphore *joinsem; //  Dung de dong bo viec thuc thi va dung lai cua 1 tien trinh  
    Semaphore *exitsem; //  Ket thuc tien trinh va giai phong tai nguyen
    Semaphore *multex;  //  Dong bo viec su dung chung tai nguyen giua nhieu tien trinh

    int exitcode;
    int numwait;

    Thread *thread;
    char filename[128];

   public:
    int parentID;
    int processID;
    PCB();
    PCB(int id);
    ~PCB();

    int Exec(char *filename, int pid);
    int GetID();
    int GetNumWait();

    void JoinWait();
    void ExitWait();
    void JoinRelease();
    void ExitRelease();

    void IncNumWait();
    void DecNumWait();

    void SetExitCode(int ec);
    int GetExitCode();

    void SetFileName(char *fn);
    char *GetFileName();
};

#endif
