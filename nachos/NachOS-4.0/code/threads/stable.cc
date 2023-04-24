#include "stable.h"
#include "synch.h"
#include "string.h"

STable::STable()
{
    this->bm = new Bitmap(MAX_SEMAPHORE);

    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        this->semTab[i] = NULL;
    }
}

STable::~STable()
{
    if (this->bm)
    {
        delete this->bm;
        this->bm = NULL;
    }
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (this->semTab[i])
        {
            delete this->semTab[i];
            this->semTab[i] = NULL;
        }
    }
}

int STable::Create(char *name, int init)
{

    // Check da co semaphore nay hay chua?
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (bm->Test(i))
        {
            if (strcmp(name, semTab[i]->GetName()) == 0)
            {
                return -1;
            }
        }
    }
    // Tim slot cong trong tren bang semTab
    int id = this->FindFreeSlot();

    // k thay id
    if (id < 0)
    {
        return -1;
    }

    // Neu tim thay slot trong thi nap Semaphore vao semTab[id]
    this->semTab[id] = new Sem(name, init);
    return 0;
}

int STable::Wait(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        // Kiem tra o thu i da duoc nap semaphore chua
        if (bm->Test(i))
        {
            // Neu co thi tien hanh so sanh name 
            // voi name cua semaphore trong semTab
            if (strcmp(name, semTab[i]->GetName()) == 0)
            {
               // Neu co semaphore thi giam xuong;
                semTab[i]->wait();
                return 0;
            }                
        }
    }
    cout << "Semaphore khong ton tai\n";
    return -1;
}

int STable::Signal(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        // Kiem tra o thu i da duoc nap semaphore chua
        if (bm->Test(i))
        {
            // Neu co thi tien hanh so sanh name voi name 
            // cua semaphore trong semTab
            if (strcmp(name, semTab[i]->GetName()) == 0)
            {
                // Neu co semaphore thi tang
                semTab[i]->signal();
                return 0;
            }
        }
    }
    cout<<"Semaphore không tồn tại!!!\n";
    return -1;
}

int STable::FindFreeSlot()
{
    return this->bm->FindAndSet();
}