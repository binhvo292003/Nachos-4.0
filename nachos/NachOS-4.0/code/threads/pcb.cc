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

void StartProcess_2(void *pid)
{
    int id;
    id = *((int *)pid);
    // Lay fileName cua process id nay
    char *fileName = kernel->pTab->GetFileName(id);
    cout<<kernel->pTab->GetFileName(id);
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
    // Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
    multex->P();

    this->thread = new Thread(filename);
    if (this->thread == NULL)
    {
        printf("\nPCB::Exec: Not enough memory!\n");
        multex->V(); // Nha CPU de nhuong CPU cho tien trinh khac
        return -1;   // Tra ve -1 neu that bai
    }

    //  Đặt processID của thread này là id.
    this->thread->processID = id;
    // Đặt parrentID của thread này là processID của thread gọi thực thi Exec
    this->parentID = kernel->currentThread->processID;
    // Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int,
    // sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.

    // Không được sử dụng biến id ở đây, vì biến id là biến cục bộ,
    // nên khi hàm này kết thúc thì giá trị của biến này cũng bị xóa

    this->thread->Fork(StartProcess_2, &this->thread->processID);

    multex->V();
    
    return id;
}

int PCB::GetID() { return thread->processID; }

int PCB::GetNumWait() { return numwait; }

void PCB::JoinWait()
{
    // Gọi joinsem->P() để tiến trình chuyển sang trạng thái block và ngừng lại,
    // chờ JoinRelease để thực hiện tiếp.
    joinsem->P();
}

void PCB::ExitWait()
{
    // Gọi exitsem-->V() để tiến trình chuyển sang trạng thái block và ngừng
    // lại, chờ ExitReleaseđể thực hiện tiếp.
    exitsem->P();
}

void PCB::JoinRelease()
{
    // Gọi joinsem->V() để giải phóng tiến trình gọi JoinWait().
    joinsem->V();
}

void PCB::ExitRelease()
{
    // Gọi exitsem->V() để giải phóng tiến trình đang chờ.
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
