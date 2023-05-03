#include "sem.h"
#include "synch.h"

Sem::Sem(char *nameInit, int i)
{
    strcpy(this->name, nameInit);
    sem = new Semaphore(this->name, i);
}

Sem::~Sem()
{
    if (sem)
        delete sem;
}

void Sem::wait() { sem->P(); } // Down(sem)

void Sem::signal() { sem->V(); } // Up(sem)

char *Sem::GetName() { return this->name; }