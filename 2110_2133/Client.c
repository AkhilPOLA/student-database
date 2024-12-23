#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "header.h"
#include "Client.h"

int connfd = -1;  

void connectSocket(const char *serv_addr, const char *serv_port)
{
    struct sockaddr_in servaddr;

    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed to create socket\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    if (inet_aton(serv_addr, &servaddr.sin_addr) == 0)
    {
        fprintf(stderr, "Invalid IP address: %s\n", serv_addr);
        exit(1);
    }

    servaddr.sin_port = htons(atoi(serv_port));

    if (connect(connfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        fprintf(stderr, "Failed to connect to server\n");
        exit(1);
    }

    printf("Connected to server at %s:%s\n", serv_addr, serv_port);
}

void closeFd()
{
    if (connfd != -1)
    {
        if (close(connfd) == -1)
        {
            fprintf(stderr, "Error closing connection from client: %s\n", strerror(errno));
        }
        else
        {
            printf("Connection closed successfully from client\n");
        }
        connfd = -1;
    }
    else
    {
        fprintf(stderr, "Socket was not open, cannot close\n");
    }
}

void printResponse(Status responseStatus) {
    if (responseStatus == SUCCESS) {
        printf("Response from server: SUCCESS\n");
    } else {
        printf("Response from server: FAILURE\n");
    }
}

void addStudent(int roll_No, char name[100], float cgpa) {
    Data record;
    record.Opr = ADDSTUDENT;
    record.Roll_no = roll_No;
    strcpy(record.Name, name);
    record.CGPA = cgpa;

    if (write(connfd, &record, sizeof(record)) == -1) {
        printf("Error while writing to socket\n");
    }

    Status responseStatus;
    if (recv(connfd, &responseStatus, sizeof(Status), 0) <= 0) {
        perror("Failed to receive response from server");
    } else {
        if (responseStatus == SUCCESS) {
            printf("Successfully added student with Roll No: %d, Name: %s, CGPA: %.2f\n", record.Roll_no, record.Name, record.CGPA);
        } else {
            printf("Can't add student with roll number %d as this number already exists\n", record.Roll_no);
        }
    }
}

void modifyStudent(int roll_No, float cgpa) {
    Data record;
    record.Opr = MODIFYSTUDENT;
    record.Roll_no = roll_No;
    record.CGPA = cgpa;

    if (write(connfd, &record, sizeof(record)) == -1) {
        printf("Error while writing to socket\n");
    }

    Status responseStatus;
    if (recv(connfd, &responseStatus, sizeof(Status), 0) <= 0) {
        perror("Failed to receive response from server");
    } else {
        if (responseStatus == SUCCESS) {
            printf("Successfully modified CGPA of student with Roll No: %d to %.2f\n", record.Roll_no, record.CGPA);
        } else {
            printf("Can't modify CGPA of student with roll number %d as this student does not exist\n", record.Roll_no);   
        }
    }
}

void deleteStudent(int roll_No) {
    Data record;
    record.Opr = DELETESTUDENT;
    record.Roll_no = roll_No;

    if (write(connfd, &record, sizeof(record)) == -1) {
        printf("Error while writing to socket\n");
    }

    Status responseStatus;
    if (recv(connfd, &responseStatus, sizeof(Status), 0) <= 0) {
        perror("Failed to receive response from server");
    } else {
        if (responseStatus == SUCCESS) {
            printf("Successfully deleted student with Roll No: %d \n", record.Roll_no);
        } else {
            printf("Can't delete student with Roll No: %d as this student record does not exist \n", record.Roll_no);   
        }
    }
}

void addCourse(int roll_No, int courseCode, int marks) {
    Data record;
    record.Opr = ADDCOURSE;
    record.Roll_no = roll_No;
    record.Course_Code = courseCode;
    record.Marks = marks;

    if (write(connfd, &record, sizeof(record)) == -1) {
        printf("Error while writing to socket\n");
    }

    Status responseStatus;
    if (recv(connfd, &responseStatus, sizeof(Status), 0) <= 0) {
        perror("Failed to receive response from server");
    } else {
        if (responseStatus == SUCCESS) {
            printf("Successfully added the course with course code %d and marks %d for student with roll number %d \n", record.Course_Code,record.Marks,record.Roll_no );
        }
        else if(responseStatus == FAILURECOURSE) {
            printf("Can't add course for student with roll number %d as course with course code %d already exists \n", record.Roll_no, record.Course_Code);
        }
        else{
            printf("Can't add course with course code %d for student with roll number %d as this student does not exist \n", record.Course_Code, record.Roll_no);
        }
    }
}

void modifyCourse(int roll_No, int courseCode, int marks) {
    Data record;
    record.Opr = MODIFYCOURSE;
    record.Roll_no = roll_No;
    record.Course_Code = courseCode;
    record.Marks = marks;

    if (write(connfd, &record, sizeof(record)) == -1) {
        printf("Error while writing to socket\n");
    }

    Status responseStatus;
    if (recv(connfd, &responseStatus, sizeof(Status), 0) <= 0) {
        perror("Failed to receive response from server");
    } else {
        if (responseStatus == SUCCESS) {
            printf("Successfully modified the marks of student with roll number %d in a course with course code %d to %d \n", record.Roll_no, record.Course_Code,record.Marks );
        }
        else if(responseStatus == FAILURECOURSE) {
            printf("Can't modify the marks of student with roll number %d in a course with course code %d as course does not exist \n", record.Roll_no, record.Course_Code);
        }
        else{
            printf("Can't modify the marks of student with roll number %d in a course with course code %d as student does not exist\n", record.Roll_no, record.Course_Code);
        }
    }
}

void deleteCourse(int roll_No, int courseCode) {
    Data record;
    record.Opr = DELETECOURSE;
    record.Roll_no = roll_No;
    record.Course_Code = courseCode;

    if (write(connfd, &record, sizeof(record)) == -1) {
        printf("Error while writing to socket\n");
    }

    Status responseStatus;
    if (recv(connfd, &responseStatus, sizeof(Status), 0) <= 0) {
        perror("Failed to receive response from server");
    } else {
        if (responseStatus == SUCCESS) {
            printf("Successfully deleted the course with code %d for student with roll number %d \n", record.Course_Code,record.Roll_no);
        }
        else if(responseStatus == FAILURECOURSE) {
            printf("Can't delete the course with course code %d for student with roll number %d as course does not exist\n",  record.Course_Code,record.Roll_no);
        }
        else{
            printf("Can't delete the course with course code %d for student with roll number %d as student does not exist\n",  record.Course_Code,record.Roll_no);
        }
    }
}


