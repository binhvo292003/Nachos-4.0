#include "ptable.h"
#include "synch.h"
#define For(i, a, b) for (int i = (a); i < b; ++i)

PTable::PTable(int size)
{
    // Kiem tra kich thuoc cua bang coi co phu hop khong
    if (size < 0)
        return;

    psize = size;
    //  Quan li tien trinh cua thread 
    bm = new Bitmap(size);
    //  Quan li dong bo cho tien trinh
    bmsem = new Semaphore("bmsem", 1);

    for(int i = 0;i < MAX_PROCESS;++i){
        pcb[i] = NULL;
    }

    //  Danh dau vi tri ban dau cua mang bm
    bm->Mark(0);

    //  Mang qua li cac tien trinh dang chay
    pcb[0] = new PCB(0);
    pcb[0]->parentID = -1;
}

PTable::~PTable()
{
    if (bm != 0)
        delete bm;

    For(i, 0, psize)
    {
        if (pcb[i] != 0)
            delete pcb[i];
    }

    delete bmsem;
}

int PTable::ExecUpdate(char *name)
{

    DEBUG(dbgSys, "go to PTable::ExecuteUpdate");
    // Tranh viec nap 2 tien trinh cung luc
    bmsem->P();
    // Kiem tra ton tai cua file name
    if (name == NULL){
        printf("\nptable::Exec : Can't not execute name is NULL.\n");
        //  Giai phong tien trin
        bmsem->V();
        return -1;  
    }
    // Kiem tra de chuong trinh khong thuc thi chinh nho
    if (strcmp(name, kernel->currentThread->getName()) == 0 
        || strcmp(name, kernel->currentThread->getName()) == 0){
        printf("\nPTable::Exec : Can't not execute itself.\n");
        bmsem->V();
        return -1;
    }

    // Tim vi tri trong trong bang
    int index = this->GetFreeSlot();

    // Check if have free slot
    if (index < 0)
    {
        printf("\nPTable::Exec :There is no free slot.\n");
        bmsem->V();
        return -1;
    }

    // NTao PCB moi neu co cho? trong' voi processID chinh la index 
    pcb[index] = new PCB(index);
    pcb[index]->SetFileName(name);

    // parrentID la processID cua currentThread
    pcb[index]->parentID = kernel->currentThread->processID;

    // Exec PCB nay
    int pid = pcb[index]->Exec(name, index);

    // Tang semaphore
    bmsem->V();

    //  Tra ve processID
    return pid;
}

int PTable::JoinUpdate(int id)
{
    // Kiem tra id co hop le khong
    if (id < 0 || id >= psize || pcb[id] == NULL)
    {
        printf("\nPTable::JoinUpdate : Can not join.", id);
        return -1;
    }
    // Check if process running is parent process of process which joins
    if (kernel->currentThread->processID != pcb[id]->parentID)
    {
        printf("\nPTable::JoinUpdate Can't join process which is not its parent process.\n");
        return -1;
    }

    // Tang numwait và goi JoinWait() de cho tien trinh con thuc hien
    pcb[pcb[id]->parentID]->IncNumWait();


    pcb[id]->JoinWait();

    // xu li exist code
    int exit_code = pcb[id]->GetExitCode();
    // ExitRelease() de cho tien trinh con thoat
    pcb[id]->ExitRelease();

    // tra ve exit code neu thanh cong
    return exit_code;
}
int PTable::ExitUpdate(int exitcode)
{
    // Neu tien trinh la main thi Halt() cho user
    int pid = kernel->currentThread->processID;
    if (pid == 0){
        delete kernel->currentThread;
        kernel->interrupt->Halt();
        return 0;
    }

    //  Kiem tra pid co hop le khong
    if (IsExist(pid) == false){
        printf("\nPTable::ExitUpdate: This %d is not exist!!!", pid);
        return -1;
    }

    // Neu chuong trinh hop le thi SetExitCode de dat exitcode cho tien trinh
    pcb[pid]->SetExitCode(exitcode);
    pcb[pcb[pid]->parentID]->DecNumWait();

    // Giai phong va thoat vung nho
    pcb[pid]->JoinRelease();
    pcb[pid]->ExitWait();
    Remove(pid);
   
    return exitcode;
}

// Find free slot in order to save the new process infom
int PTable::GetFreeSlot()
{
    return bm->FindAndSet();
}

// Kiem tra ID co ton tai khong
bool PTable::IsExist(int pid)
{
    return bm->Test(pid);
}

// Xoa processID khi da thuc thi xong
void PTable::Remove(int pid)
{
    bm->Clear(pid);
    if (pcb[pid] != 0){
        delete pcb[pid];
        pcb[pid] = NULL;
    }
}

char *PTable::GetFileName(int id)
{
    return (pcb[id]->GetFileName());
}