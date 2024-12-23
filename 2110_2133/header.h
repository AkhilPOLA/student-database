#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
typedef enum Operation {
    ADDSTUDENT,
    MODIFYSTUDENT,
    DELETESTUDENT,
    ADDCOURSE,
    MODIFYCOURSE,
    DELETECOURSE,
    WRITE
} Operation;

typedef enum Status {
    SUCCESS,
    FAILURESTUDENT, // refers to failure because of student record
    FAILURECOURSE // refers to failure because of course record
} Status;

#define FILE_MODE (S_IRUSR | S_IWUSR)

typedef struct Data {
    enum Operation Opr;   
    enum Status Stat;     
    int Roll_no;         
    char Name[100];       
    int Course_Code;   
    float CGPA;   
    int Marks;            
} Data;



