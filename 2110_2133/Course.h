typedef struct Course
{
    int Course_Code;
    int Marks;
    struct Course *next;
    struct Course *previous;
} Course;
int addCourse(int roll_No, int courseCode, int marks);
int modifyCourse(int roll_No, int courseCode, int marks);
int deleteCourse(int roll_No, int courseCode);