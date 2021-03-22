#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <bits/stdc++.h>
using namespace std;
struct rsa_key {
  long long int private_key;
  long long int public_key;
};

struct user {
  string userName;
  string password;
  int isAdmin;
  int isStudent;
  int isTeacher;
  string userId;
  string user_type;
  struct rsa_key rsa_keys;
};

struct myUsers {
  struct user users[1000];
  int currentUserIndex;
};

struct object {
  string objectId;
  struct user objectOwner;
  string objectName;
  string filetype;
  string currentPermission;
};

struct myObjects {
  struct object objects[1000];
  int currentObjectIndex;
};

struct system {
  int authUserId;
  struct myUsers myUsers;
  struct myObjects myObjects;
};


int getIndexFromSystem(struct system * mySystem, string type, string id);

void listCourseOrTypeObjects(struct system * mySystem, string type);

void viewCourseOrTeacherObjects(struct system * mySystem, string type);

void writeCourseOrTeacherObjects(struct system * mySystem, string type);

void listStudentObjects(struct system * mySystem, int userId = 0);

string getUserStatus(struct user user);

void displayUserDetails(struct system * mySystem, string userName);

struct user getCurrentUserDetails(struct system * mySystem);

void userLogin(struct system * mySystem);

void addUserToSystem(struct system * mySystem, string userType);

struct system createSystem();

string gen_id();

bool studentFileExist(struct system * mySystem, string fileName, int userId = 0);

void addStudentObject(struct system * mySystem, int userId = 0);

void studentWriteObject(struct system * mySystem, int userId = 0);

bool courseOrTeacherFileExist(struct system * mySystem, string id, string type);

void studentReadObject(struct system * mySystem, int userId=0);

// List All Available Students.
void listStudents(struct system * mySystem);
// List All Available Teachers.
void listTeachers(struct system * mySystem);

void addTeacherObject(struct system * mySystem, string fileLocation);

void logout(struct system * mySystem);

void displayAllObjects(struct system * mySystem);

string encryptMessage(string message, int key);

string decryptMessage(string message, int key);
#endif

