#include <stdio.h>
#include "Student.h"
#include "header.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#define SHARED_MEM "/readCount"
#define SEM_MUTEX "/mutex"
#define SEM_WRITELOCK "/writeLock"
int *readCount;
int shared_fd = -1;
sem_t *mutex;
sem_t *writeLock;
char *outputFileName = "2110_2133.out";
Data record;

void signalhandler(int signum)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("Child %d terminated\n", pid);
    }
}
void cleanup_resources()
{
    if (readCount)
    {
        munmap(readCount, sizeof(int));
    }
    if (shared_fd != -1)
    {
        close(shared_fd);
    }
    if (mutex)
    {
        sem_close(mutex);
        sem_unlink(SEM_MUTEX);
    }
    if (writeLock)
    {
        sem_close(writeLock);
        sem_unlink(SEM_WRITELOCK);
    }

    shm_unlink(SHARED_MEM);
}
void initialize_resources()
{
    shared_fd = shm_open(SHARED_MEM, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (shared_fd == -1)
    {
        perror("shm_open failed");
        exit(1);
    }

    if (ftruncate(shared_fd, sizeof(int)) == -1)
    {
        perror("ftruncate failed");
        cleanup_resources();
        exit(1);
    }

    readCount = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (readCount == MAP_FAILED)
    {
        perror("mmap failed");
        cleanup_resources();
        exit(1);
    }

    *readCount = 0;

    mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);
    if (mutex == SEM_FAILED)
    {
        cleanup_resources();
        perror("mutex open failed");
        exit(1);
    }
    writeLock = sem_open(SEM_WRITELOCK, O_CREAT, 0666, 1);
    if (writeLock == SEM_FAILED)
    {
        cleanup_resources();
        perror("writelock open failed");
        exit(1);
    }
}

void wait_for_semaphore(sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("sem_wait failed");
        exit(1);
    }
}

void signal_semaphore(sem_t *sem)
{
    if (sem_post(sem) == -1)
    {
        perror("sem_post failed");
        exit(1);
    }
}

void parse(int connfd)
{
    Status responseStatus = SUCCESS;
    int a;
    switch (record.Opr)
    {
    case ADDSTUDENT:
        if (addStudent(record.Roll_no, record.Name, record.CGPA) == -1)
        {
            responseStatus = FAILURESTUDENT;
        }
        break;
    case MODIFYSTUDENT:
        if (modifyStudent(record.Roll_no, record.CGPA) == -1)
        {
            responseStatus = FAILURESTUDENT;
        }
        break;
    case DELETESTUDENT:
        if (deleteStudent(record.Roll_no) == -1)
        {
            responseStatus = FAILURESTUDENT;
        }
        break;
    case ADDCOURSE:
        if ((a = addCourse(record.Roll_no, record.Course_Code, record.Marks)) == -1)
        {
            responseStatus = FAILURECOURSE;
        }
        else if (a == -2)
        {
            responseStatus = FAILURESTUDENT;
        }
        break;
    case MODIFYCOURSE:
        if ((a = modifyCourse(record.Roll_no, record.Course_Code, record.Marks)) == -1)
        {
            responseStatus = FAILURECOURSE;
        }
        else if (a == -2)
        {
            responseStatus = FAILURESTUDENT;
        }
        break;
    case DELETECOURSE:
        if ((a = deleteCourse(record.Roll_no, record.Course_Code)) == -1)
        {
            responseStatus = FAILURECOURSE;
        }
        else if (a == -2)
        {
            responseStatus = FAILURESTUDENT;
        }
        break;
    default:
        responseStatus = FAILURESTUDENT;
        break;
    }

    if (send(connfd, &responseStatus, sizeof(responseStatus), 0) < 0)
    {
        perror("Failed to send response to client");
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        return 1;
    }
    if (signal(SIGCHLD, signalhandler) == SIG_ERR)
    {
        fprintf(stderr, "error installing handler for SIGCHLD\n");
        exit(1);
    }
    int serv_port = atoi(argv[1]);
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    initialize_resources();

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create socket");
        cleanup_resources();
        exit(1);
    }

    int opt = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        cleanup_resources();
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(serv_port);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("Failed to bind socket");
        cleanup_resources();
        exit(1);
    }

    if (listen(listenfd, 5) == -1)
    {
        perror("Failed to listen");
        cleanup_resources();
        exit(1);
    }

    printf("Server is running on port %d...\n", serv_port);

    while (1)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0)
        {
            if (errno == EINTR)
                continue;
            perror("Failed to accept connection");
            break;
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Error forking");
            break;
        }
        else if (pid == 0)
        {
            if (close(listenfd) == -1)
            {
                fprintf(stderr, "Error closing socket fd from child process, pid= %d\n", getpid());
                exit(1);
            }
            printf("Child process created with pid = %d for client %s %d\n", getpid(), inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
            wait_for_semaphore(mutex);
            (*readCount)++;
            // printf("readd count after incrementing %d from pid=%d\n", *readCount,getpid());
            if (*readCount == 1)
            {
                wait_for_semaphore(writeLock);
                // printf("write is blocked from pid=%d\n", getpid());
            }
            signal_semaphore(mutex);
            fileParse();
            wait_for_semaphore(mutex);
            (*readCount)--;
            // printf("readd count after decrementing %d from pid=%d\n", *readCount,getpid());
            
            if (*readCount == 0)
            {
                signal_semaphore(writeLock);
                // printf("write is unblocked from pid=%d\n", getpid());
            }
            signal_semaphore(mutex);
            ssize_t bytesRead;
            while ((bytesRead = read(connfd, &record, sizeof(Data))) > 0)
            {
                parse(connfd);
            }

            if (bytesRead == -1)
            {
                perror("Error reading from socket");
            }
            wait_for_semaphore(writeLock);
            // printf("writelock is blocked from pid=%d\n", getpid());
            writer();
            signal_semaphore(writeLock);
            // printf("writelock is un blocked from pid=%d\n", getpid());
            if (close(connfd) == -1)
            {
                fprintf(stderr, "Error closing connection of client %s %d from child process whose pid=%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port, getpid());
                exit(1);
            }
            printf("Connection Closed from client %s %d from child process whose pid = %d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port, getpid());
            exit(0);
        }
        if (close(connfd) == -1)
        {
            fprintf(stderr, "Error closing connection of client %s %d from parent process\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
            exit(1);
        }
    }
    cleanup_resources();
    if (close(listenfd) == -1)
    {
        fprintf(stderr, "Error closing socket fd from parent process= %d\n", getpid());
        exit(1);
    }
    return 0;
}
