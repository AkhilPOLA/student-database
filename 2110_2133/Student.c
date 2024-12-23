#include <stdio.h>
#include "Student.h"
#include <stdlib.h>
#include <string.h>
Student *head = NULL;
Student *end = NULL;
Student *searchStudent(int roll_No)
{
  Student *search = head;
  while (search != NULL && search->Roll_no != roll_No)
  {
    search = search->next;
  }
  if (search != NULL && search->Roll_no == roll_No)
  {
    return search;
  }
  return NULL;
}
int addStudent(int roll_No, char name[100], float cgpa)
{
  if (searchStudent(roll_No) == NULL)
  {
    Student *newStudent = (Student *)malloc(sizeof(Student));
    newStudent->Roll_no = roll_No;
    strcpy(newStudent->Name, name);
    newStudent->CGPA = cgpa;
    newStudent->No_of_subjects = 0;
    newStudent->CourseHead = NULL;
    newStudent->next = NULL;
    newStudent->previous = NULL;
    if (head == NULL)
    {
      head = newStudent;
      end = head;
    }
    else
    {
      end->next = newStudent;
      newStudent->previous = end;
      end = newStudent;
    }
    return 0;
  }
  else
  {
    // printf("Can't add student with roll number %d as this number already exists\n", roll_No);
    return -1;
  }
}
int modifyStudent(int roll_No, float cgpa)
{
  Student *search;
  if ((search = searchStudent(roll_No)) != NULL)
  {
    search->CGPA = cgpa;
    return 0;
  }
  else
  {
    // printf("can't modifty student with roll number %d as student doesn't exist\n", roll_No);
    return -1;
  }
}
int deleteStudent(int roll_No)
{
  Student *search;
  if ((search = searchStudent(roll_No)) != NULL)
  {
    if (search->previous == NULL)
    {
      search = search->next;
      if (search != NULL)
      {
        search->previous = NULL;
      }
      head = search;
    }
    else if (search->next == NULL)
    {
      (search->previous)->next = NULL;
    }
    else
    {
      (search->next)->previous = search->previous;
      (search->previous)->next = search->next;
    }
    return 0;
  }
  else
  {
    // printf("can't delete student with roll number %d as student doesn't exist\n", roll_No);
    return -1;
  }
}
