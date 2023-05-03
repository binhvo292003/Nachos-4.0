#include "synch.h"
#include "pcb.h"

PCB::PCB(int id)
{
    this->processID = kernel->currentThread->processID;
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    multex = new Semaphore("multex", 1);
}

PCB::~PCB()
{
    delete joinsem;
    delete exitsem;
    delete multex;

    if (thread)
    {
        thread->Finish();
        delete thread;
    }

    delete[] filename;
}

// Khoi tao tien trinh moivoi pid truyen vao
void StartProcess(void *pid)
{
    int id;
    id = *((int *)pid);
    // Lay fileName cua process id nay
    char *fileName = kernel->pTab->GetFileName(id);
    AddrSpace *space;
    space = new AddrSpace(fileName);

    if (space == NULL)
    {
        printf("\nPCB::Exec: Can't create AddSpace.");
        return;
    }

    space->Execute();

    ASSERT(FALSE); // machine->Run never returns;
                   // the address space exits
                   // by doing the syscall "exit"
}

int PCB::Exec(char *filename, int id)
{
    // Goi multex de nap duy nhat 1 tien trinh tai 1 thoi diem
    multex->P();

    this->thread = new Thread(filename);
    if (this->thread == NULL)
    {
        printf("\nPCB::Exec: Not enough memory!\n");
        multex->V(); // Nhuong CPU cho tien trinh khac
        return -1;   // Tra ve -1 neu that bai
    }

    //  Dat processID cua thread nay bang voi id truyen vao
    this->thread->processID = id;
    //  Dat parentID la pid cua thread goi tien trinh nay
    this->parentID = kernel->currentThread->processID;

    // Khong dung bien id tai day vi id la bien cuc bo
    this->thread->Fork(StartProcess, &this->thread->processID);
    multex->V();

    return id;
}

int PCB::GetID() { return thread->processID; }

int PCB::GetNumWait() { return numwait; }

void PCB::JoinWait()
{
    // Goi joinsem->P() de dung lai
    joinsem->P();
}

void PCB::ExitWait()
{
    exitsem->P();
}

void PCB::JoinRelease()
{
    // Goi joinsem->V() giai phong tien trinh goi JoinWait().
    joinsem->V();
}

void PCB::ExitRelease()
{
    exitsem->V();
}

void PCB::IncNumWait()
{
    multex->P();
    ++numwait;
    multex->V();
}

void PCB::DecNumWait()
{
    multex->P();
    if (numwait > 0)
        --numwait;
    multex->V();
}

void PCB::SetExitCode(int ec)
{
    exitcode = ec;
}

int PCB::GetExitCode()
{
    return exitcode;
}

void PCB::SetFileName(char *fn)
{
    strcpy(filename, fn);
}

char *PCB::GetFileName()
{
    return filename;
}
