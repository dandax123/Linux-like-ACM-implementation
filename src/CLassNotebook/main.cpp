#include <bits/stdc++.h>
#include "functions/functions.h"
using namespace std;


int main() {
  cout << "\t\t\t\t\t## Welcome to the M.T. Class Notebook-like system ##" << endl;
  cout << "\nSystem Description:\n\t\t We build a simple M.T. Class Notebook like system,\n";
  cout << "where it have 3 types of actors, System Adminstrator, Teachers, and Students." << endl;
  cout << "\nCreate a new Default Account" << endl;
  struct system mySystem = createSystem();
  struct system* mySystemPointer = &mySystem;
  addUserToSystem(mySystemPointer, "admin"); // Adding an admin user 
  system("CLS"); // Clearing the screen.

  int action1 = 1;
  while (action1 > 0) {
    if (mySystem.authUserId < 0) {
      cout << endl << "1. Login" << endl;
    }
    if (mySystem.authUserId >= 0) {
      cout << "2. Logout" << endl;
    }
    cout << "0. Quit" << endl;
    cin >> action1;
    if (action1 <= 0) {
      exit(1);
    }
    if (action1 == 1 && mySystem.authUserId < 0) {
      int action2 = 1;
      userLogin(mySystemPointer);
      struct user currentUser = getCurrentUserDetails(mySystemPointer);

      // admin menu
      if (currentUser.isAdmin && mySystem.authUserId >= 0) {
        while (action2 > 0) {
          cout << "# Admin Menu" << endl;
          cout << "1. Add new User" << endl;
          cout << "2. List Users" << endl;
          cout << "3. Manage Materials" << endl;
          cout << "0. Return" << endl;
          cin >> action2;
          if (action2 == 1) {
            int actionAdd = 1;
            while (actionAdd) {
              cout << "1. Add new Teacher" << endl;
              cout << "2. Add new Student" << endl;
              cout << "0. Return" << endl;
              cin >> actionAdd;
              if (actionAdd == 1) {
                addUserToSystem(mySystemPointer, "teacher");
              } else if (actionAdd == 2) {
                addUserToSystem(mySystemPointer, "student");
              } else break;
            }
          } else if (action2 == 2) {
            int actionList = 1;
            while (actionList) {
              cout << "1. List all Teachers" << endl;
              cout << "2. List all Students" << endl;
              cout << "0. Return" << endl;
              cin >> actionList;
              if (actionList == 1) {
                listTeachers(mySystemPointer);
              } else if (actionList == 2) {
                listStudents(mySystemPointer);
              } else break;
            }
          } else if (action2 == 3) {
            int actionManage = 1;
            while (actionManage) {
              cout << "1. Manage Teacher Materials" << endl;
              cout << "2. Manage Course Materials" << endl;
              cout << "3. Manage Student Materials" << endl;
              cout << "0. Return" << endl;
              cin >> actionManage;
              if (actionManage == 1) {
                int actionTeacher = 1;
                while (actionTeacher) {
                  cout << "1. Create new Teacher material" << endl;
                  cout << "2. Write to Teacher Material" << endl;
                  cout << "3. Read from Teacher Material" << endl;
                  cout << "0. Return" << endl;
                  cin >> actionTeacher;
                  if (actionTeacher == 1) {
                    addTeacherObject(mySystemPointer, "teacher_materials");
                  }
                  if (actionTeacher == 2) {
                    writeCourseOrTeacherObjects(mySystemPointer, "teacher");
                  }
                  if (actionTeacher == 3) {
                    viewCourseOrTeacherObjects(mySystemPointer, "teacher");
                  } else break;
                }
              } else if (actionManage == 2) {
                int actionCourse = 1;
                while (actionCourse) {
                  cout << "1. Create new Course material" << endl;
                  cout << "2. Write to Course Material" << endl;
                  cout << "3. Read from Course Material" << endl;
                  cout << "0. Return" << endl;
                  cin >> actionCourse;
                  if (actionCourse == 1) {
                    addTeacherObject(mySystemPointer, "course_materials");
                  }
                  if (actionCourse == 2) {
                    writeCourseOrTeacherObjects(mySystemPointer, "course");
                  }
                  if (actionCourse == 3) {
                    viewCourseOrTeacherObjects(mySystemPointer, "course");
                  } else break;
                }
              } else if (actionManage == 3) {
                int actionStudent = 1;
                while (actionStudent) {
                  cout << "1. List student files" << endl;
                  cout << "2. Create student file" << endl;
                  cout << "3. Read student file" << endl;
                  cout << "4. Write to a student file" << endl;
                  cout << "0. Return" << endl;
                  cin >> actionStudent;
                  if (actionStudent == 1) {
                    listStudentObjects(mySystemPointer);
                  }
                  if (actionStudent == 2) {
                    addTeacherObject(mySystemPointer, "student_materials");
                  }
                  if (actionStudent == 3) {
                    studentReadObject(mySystemPointer);
                  }
                  if (actionStudent == 4) {
                    studentWriteObject(mySystemPointer);
                  } else break;
                }
              } else break;
            }
          }
        }
      }

      //student menu
      else if (currentUser.isStudent && mySystem.authUserId >= 0) {
        while (action2 > 0 && mySystem.authUserId >= 0) {
          cout << "# Student Menu" << endl;
          cout << "1. View Course Material " << endl;
          cout << "2. Manage Student Materials" << endl;
          cout << "0. Return" << endl;
          cin >> action2;
          if (action2 == 1) {
            viewCourseOrTeacherObjects(mySystemPointer, "course");
          }
          if (action2 == 2) {
            int actionManage = 1;
            while (actionManage) {
              cout << "1. List Files" << endl;
              cout << "2. Create File" << endl;
              cout << "3. Read File" << endl;
              cout << "4. Write to File" << endl;
              cout << "0. Return" << endl;
              cin >> actionManage;
              if (actionManage == 1) {
                listStudentObjects(mySystemPointer, mySystem.authUserId);
              }
              if (actionManage == 2) {
                addStudentObject(mySystemPointer);
              }
              if (actionManage == 3) {
                studentReadObject(mySystemPointer, mySystem.authUserId);
              }
              if (actionManage == 4) {
                studentWriteObject(mySystemPointer, mySystem.authUserId);
              } else break;
            }
          }
        }
      }

      //teacher menu
      else if (currentUser.isTeacher && mySystem.authUserId >= 0) {
        while (action2 > 0 && mySystem.authUserId >= 0) {
          cout << "# Teacher Menu" << endl;
          cout << "1. Manage Course Materials" << endl;
          cout << "2. Manage Teacher Materials" << endl;
          cout << "3. Manage Student Materials" << endl;
          cout << "0. Return" << endl;
          cin >> action2;
          if (action2 == 1) {
            int actionCourse = 1;
            while (actionCourse) {
              cout << "1. Create new Course material" << endl;
              cout << "2. Write to Course Material" << endl;
              cout << "3. Read from Course Material" << endl;
              cout << "0. Return" << endl;
              cin >> actionCourse;
              if (actionCourse == 1) {
                addTeacherObject(mySystemPointer, "course_materials");
              }
              if (actionCourse == 2) {
                writeCourseOrTeacherObjects(mySystemPointer, "course");
              }
              if (actionCourse == 3) {
                viewCourseOrTeacherObjects(mySystemPointer, "course");
              } else break;
            }
          }
          if (action2 == 2) {
            int actionTeacher = 1;
            while (actionTeacher) {
              cout << "1. Create new Teacher material" << endl;
              cout << "2. Write to Teacher Material" << endl;
              cout << "3. Read from Teacher Material" << endl;
              cout << "0. Return" << endl;
              cin >> actionTeacher;
              if (actionTeacher == 1) {
                addTeacherObject(mySystemPointer, "teacher_materials");
              }
              if (actionTeacher == 2) {
                writeCourseOrTeacherObjects(mySystemPointer, "teacher");
              }
              if (actionTeacher == 3) {
                viewCourseOrTeacherObjects(mySystemPointer, "teacher");
              } else break;
            }
          }
          if (action2 == 3) {
            int actionStudent = 1;
            while (actionStudent) {
              cout << "1. List Students" << endl;
              cout << "2. Manage Student Material" << endl;
              cout << "0. Return" << endl;
              cin >> actionStudent;
              if (actionStudent == 1) {
                listStudents(mySystemPointer);
              }
              if (actionStudent == 2) {
                int actionManage = 1;
                while (actionManage) {
                  cout << "1. List student files" << endl;
                  cout << "2. Create student file" << endl;
                  cout << "3. Read student file" << endl;
                  cout << "4. Write to a student file" << endl;
                  cout << "0. Return" << endl;
                  cin >> actionManage;
                  if (actionManage == 1) {
                    listStudentObjects(mySystemPointer);
                  }
                  if (actionManage == 2) {
                    addTeacherObject(mySystemPointer, "student_materials");
                  }
                  if (actionManage == 3) {
                    studentReadObject(mySystemPointer);
                  }
                  if (actionManage == 4) {
                    studentWriteObject(mySystemPointer);
                  } else break;
                }
              }
            }
          }
        }
      }
    }

    if (action1 == 2 && mySystem.authUserId >= 0) {
      logout(mySystemPointer);
    }
  }

  return 0;
}
    
