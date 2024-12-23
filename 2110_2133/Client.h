void signal_handler(int signum);
void connectSocket();
void closeFd();
void addStudent(int roll_No, char name[100], float cgpa);
void modifyStudent(int roll_No, float cgpa);
void deleteStudent(int roll_No);
void addCourse(int roll_No, int courseCode, int marks);
void modifyCourse(int roll_No, int courseCode, int marks);
void deleteCourse(int roll_No, int courseCode);
