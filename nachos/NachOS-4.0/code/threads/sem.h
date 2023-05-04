#ifndef SEM_H
#define SEM_H

class Semaphore;

class Sem {
   private:
    Semaphore* sem;
    char name[50];

   public:
    Sem(char* nameInit, int i);
    ~Sem();

    void wait();

    void signal();

    char* GetName();
};

#endif