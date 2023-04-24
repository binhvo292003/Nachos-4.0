// pcb.h 
// lớp này dùng để lưu thông tin quá đó giúp quản lý process

#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

// Process Control Block
class PCB
{
private:
    Semaphore* joinsem;         // join semaphore
    Semaphore* exitsem;         // exit semaphore 
    Semaphore* multex;          // multex semaphore

    int exitcode;		
    int numwait;                // số tiến trình đợi

    char fileName[32];          

    Thread* thread;             
public:
    int parentID;                
    char boolBG;                // Kiểm tra nếu đây là tiến trình nền
    
    PCB();
    PCB(int id);               
    ~PCB();                     

    int Exec(char* filename,int pid);        // Tạo tiến trình mới (thread)
    int GetID();                // Trả về ProcessID của tiến trình gọi thực hiện
    int GetNumWait();           // Trả về số lượng tiến trình chờ


    void JoinWait();            // Tiến trình cha đợi tiến trình con kết thúc                       
    void ExitWait();             // Tiến trình con kết thúc
    void JoinRelease();         // Báo cho tiến trình cha thực thi tiếp
    void ExitRelease();         // Cho phép tiến trình con kết thúc

    void IncNumWait();          // Tăng số tiến trình chờ
    void DecNumWait();          // Giảm số tiến trình chờ

    void SetExitCode(int exitCode);      // Đặt exitcode của tiến trình
    int GetExitCode();          // Trả về exitcode

    void SetFileName(char* filename);    // Set ten tien trinh
    char* GetFileName();        // Tra ve ten tien trinh
};

#endif // PCB_H