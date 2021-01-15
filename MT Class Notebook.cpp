#include <bits/stdc++.h>

#include <ctime>

#include <unistd.h>

#include <fstream>

#include <sstream>

using namespace std;

struct userPermission canAccessthisFile(struct system * mySystem, string objectName);
int getIndexFromSystem(struct system * mySystem, string type, string id);
void displayUserDetails(struct system * mySystem, string userName);
void displayUserDetails(struct system * mySystem, string userName);
void addUserToSystem(struct system * mySystem, string userType);
struct user getCurrentUserDetails(struct system * mySystem);
void displayAllObjects(struct system * mySystem);
void createNewObject(struct system * mySystem);
void listStudents(struct system * mySystem);
void listTeachers(struct system * mySystem);
void writeObject(struct system * mySystem);
void readObject(struct system * mySystem);
void userLogin(struct system * mySystem);
string getUserStatus(struct user user);
string encryptMessage(string msg, int key);
string decryptMessage(string msg, int key);
void logout(struct system * mySystem);
string gen_id();

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

int getIndexFromSystem(struct system * mySystem, string type, string id) {
  int index = -1;
  if (type == "users") {
    int currIndex = mySystem -> myUsers.currentUserIndex;
    for (int i = 0; i < currIndex + 1; i++) {
      if (mySystem -> myUsers.users[i].userName == id) {
        index = i;
      }
    }
  } else {
    int currIndex = mySystem -> myObjects.currentObjectIndex;
    for (int i = 0; i < currIndex + 1; i++) {
      if (mySystem -> myObjects.objects[i].objectName == id) {
        index = i;
      }
    }
  }
  return index;
}

void listCourseOrTypeObjects(struct system * mySystem, string type) {
  int currIndex = mySystem -> myObjects.currentObjectIndex;
  int i = 0;
  cout << "............................................................................." << endl;
  if (currIndex >= 0) {
    cout << "No. \t Object Name \t Object Owner " << endl;
    for (i = 0; i < currIndex + 1; i++) {
      if (mySystem -> myObjects.objects[i].filetype == type) {
        string objectName = mySystem -> myObjects.objects[i].objectName;
        string objectOwner = mySystem -> myObjects.objects[i].objectOwner.userName;
        cout << i + 1 << " \t " << objectName << " \t   " << objectOwner << endl;
      }
    }
  } else {
    cout << "No course object found" << endl;
  }
  cout << "............................................................................." << endl;
  cout << endl;
}

void viewCourseOrTeacherObjects(struct system * mySystem, string type) {
  listCourseOrTypeObjects(mySystem, type);
  string objectName;
  cout << "Enter the file name that you want to view" << endl;
  cin >> objectName;
  int objectId = getIndexFromSystem(mySystem, "objects", objectName);
  if (objectId >= 0) {
    string line;
    ifstream myfile;
    string filemainpath = type == "course" ? "course_materials/" : "teacher_materials/";
    string filePath = filemainpath + objectName + ".txt";
    myfile.open(filePath.c_str());
    int pk = mySystem->myObjects.objects[objectId].objectOwner.rsa_keys.private_key;
    cout << "............................................................................." << endl;
    cout << "Contents of the File: " << endl;
    while (getline(myfile, line)) {
		string decryptedLine = decryptMessage(line, pk);
  		cout << decryptedLine << endl;
    }
    cout << endl;
    cout << "............................................................................." << endl;
  } else {
    cout << "File doesn't exist" << endl;
  }
  return;
}

void writeCourseOrTeacherObjects(struct system * mySystem, string type) {
  listCourseOrTypeObjects(mySystem, type);
  string objectName;
  cout << "Enter the file name that you want to modify" << endl;
  cin >> objectName; 
  int objectId = getIndexFromSystem(mySystem, "objects", objectName);
  if ( objectId >= 0) {
    string line;
    ofstream myfile;
    string filemainpath = type == "course" ? "course_materials/" : "teacher_materials/";
    string filePath = filemainpath + objectName + ".txt";

    myfile.open(filePath.c_str(), std::ios_base::app);
    cout << "Type 'quit' in a new line to exit" << endl;
    int pk = mySystem->myObjects.objects[objectId].objectOwner.rsa_keys.private_key;
	do {
      getline(cin, line);
      if (line != "quit") {
      	string encryptedLine = encryptMessage(line, pk);
        myfile << encryptedLine << "\n";
      }
    }
    while (line != "quit");
    myfile.close();

  } else {
    cout << "File doesn't exist" << endl;
  }
  return;
}

void listStudentObjects(struct system * mySystem, int userId = 0) {
  if (!userId) {
    listStudents(mySystem);
    string studentName;
    cout << "Enter the student Name" << endl;
    cin >> studentName;
    userId = getIndexFromSystem(mySystem, "users", studentName);
  }
  if (userId == -1) {
    cout << "Student don't exist" << endl;
    return;
  }
  struct user currentUser = !userId ? getCurrentUserDetails(mySystem) : mySystem -> myUsers.users[userId];
  if (currentUser.isStudent) {
    int currIndex = mySystem -> myObjects.currentObjectIndex;
    int i = 0;
    cout << "............................................................................." << endl;
    if (currIndex >= 0) {
      cout << "No. \t Object Name " << endl;
      for (i = 0; i < currIndex + 1; i++) {
        if (mySystem -> myObjects.objects[i].objectOwner.userId == currentUser.userId) {
          string objectName = mySystem -> myObjects.objects[i].objectName;
          cout << i + 1 << " \t " << objectName << "\t" << endl;
        } else continue;
      }
    } else {
      cout << "No current objects" << endl;
    }
    cout << "............................................................................." << endl;
    cout << endl;
  } else cout << "Current user is not a student" << endl;
  return;
}

string getUserStatus(struct user user) {
  if (user.isAdmin) return "Admin";
  if (user.isStudent) return "Student";
  if (user.isTeacher) return "Teacher";
}

void displayUserDetails(struct system * mySystem, string userName) {
  int userId = getIndexFromSystem(mySystem, "users", userName);
  cout << "............................................................................." << endl;
  cout << "User Name: " << mySystem -> myUsers.users[userId].userName << endl;
  string accountStatus = getUserStatus(mySystem -> myUsers.users[userId]);
  cout << "User Account Type: " << accountStatus << endl;
  cout << "............................................................................." << endl;
}

struct user getCurrentUserDetails(struct system * mySystem) {
  if (mySystem -> authUserId >= 0) {
    return mySystem -> myUsers.users[mySystem -> authUserId];
  } else {
    struct user fakeUser;
    fakeUser.isAdmin = 0;
    return fakeUser;
  }
}

void userLogin(struct system * mySystem) {
  cout << "User Login " << endl;
  string userName, password;
  cout << "Enter the user name: ";
  cin >> userName;
  cout << "Enter the password: ";
  cin >> password;
  int currIndex = mySystem -> myUsers.currentUserIndex;
  int i = 0;
  bool isLoggedIn = false;
  for (i = 0; i < currIndex + 1; i++) {
    if (mySystem -> myUsers.users[i].password == password && mySystem -> myUsers.users[i].userName == userName) {
      isLoggedIn = true;
      mySystem -> authUserId = i;
    }
  }
  string loggedinState = isLoggedIn ? "login successful" : "login failed. Wrong username or password";
  cout << endl;
  cout << loggedinState << endl;

  if (isLoggedIn) {
    displayUserDetails(mySystem, userName);
  }
}

void addUserToSystem(struct system * mySystem, string userType) {
  string userName;
  cout << "Enter the " << userType << " name: ";
  cin >> userName;
  int userExists = getIndexFromSystem(mySystem, "users", userName);
  if (userExists == -1) {
    int newIndex = ++mySystem -> myUsers.currentUserIndex;
    mySystem -> myUsers.users[newIndex].userName = userName;
    cout << "Enter the " << userType << " password: ";
    cin >> mySystem -> myUsers.users[newIndex].password;
    mySystem -> myUsers.users[newIndex].isAdmin = userType == "admin" ? 1 : 0;
    mySystem -> myUsers.users[newIndex].isStudent = userType == "student" ? 1 : 0;
    mySystem -> myUsers.users[newIndex].isTeacher = userType == "teacher" ? 1 : 0;
    mySystem -> myUsers.users[newIndex].userId = gen_id();
    mySystem -> myUsers.users[newIndex].rsa_keys.private_key = (int)(1 + (rand() % (23 + 1)));
    if (userType == "student") {
      string directory = "./student_materials/" + userName + "_files";
      if (mkdir(directory.c_str()) != 0) {
        //handle it
      }
    }
    cout << "User added successfully" << endl;
    //setObjectPermission(mySystem);
  } else {
    cout << "User Already exists" << endl;
    cout << "Change the user name" << endl;
    cout << endl;
    addUserToSystem(mySystem, userType);
  }
}

struct system createSystem() {
  struct system mySystem;
  mySystem.myUsers.currentUserIndex = -1;
  mySystem.myObjects.currentObjectIndex = -1;
  mySystem.authUserId = -1;
  string directory = "course_materials";

  if (mkdir("./course_materials") != 0) {
    string a = "rmdir /s /q " + directory;
    system(a.c_str());
    mkdir("./course_materials");
  }
  if (mkdir("./teacher_materials") != 0) {
    directory = "teacher_materials";
    string a = "rmdir /s /q " + directory;
    system(a.c_str());
    mkdir("./teacher_materials");
  }
  if (mkdir("./student_materials") != 0) {
    directory = "student_materials";
    string a = "rmdir /s /q " + directory;
    system(a.c_str());
    mkdir("./student_materials");
  }
  return mySystem;
}

string gen_id() {
  const int len = 30;
  string tmp_s;
  static
  const char alphanum[] =
    "0123456789"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";

  srand((unsigned) time(NULL) * getpid());

  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i)
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

  return tmp_s;
}

bool studentFileExist(struct system * mySystem, string fileName, int userId = 0) {
  struct user currentUser = !userId ? getCurrentUserDetails(mySystem) : mySystem -> myUsers.users[userId];
  int currIndex = mySystem -> myObjects.currentObjectIndex;
  int i = 0;
  if (currIndex >= 0) {
    cout << "No \t Object Name " << endl;
    for (i = 0; i < currIndex + 1; i++) {
      if (mySystem -> myObjects.objects[i].objectOwner.userId == currentUser.userId && mySystem -> myObjects.objects[i].objectName == fileName) {
        return true;
      } else continue;
    }
  }
  return false;
}

void addStudentObject(struct system * mySystem, int userId = 0) {
  string fileName;
  cout << "Enter the file name of the object you want to create" << endl;
  cin >> fileName;
  struct user currentUser = !userId ? getCurrentUserDetails(mySystem) : mySystem -> myUsers.users[userId];
  if (studentFileExist(mySystem, fileName, userId)) {
    cout << "File already exists" << endl;
    addStudentObject(mySystem);
    return;
  }
  int newIndex = ++mySystem -> myObjects.currentObjectIndex;
  string filePath1 = "student_materials/" + currentUser.userName + "_files/" + fileName;
  string filePath2 = "student_materials/" + currentUser.userName + "_files/" + fileName + ".txt";
  ofstream file(fileName[fileName.length() - 3] != '.' ? filePath2.c_str() : filePath1.c_str());
  cout << "The file was created succesfully" << endl;
  mySystem -> myObjects.objects[newIndex].objectName = fileName;
  mySystem -> myObjects.objects[newIndex].objectId = gen_id();
  mySystem -> myObjects.objects[newIndex].objectOwner = currentUser;
  mySystem -> myObjects.objects[newIndex].filetype = "student";
  mySystem -> myObjects.objects[newIndex].currentPermission = "777";
}

void studentWriteObject(struct system * mySystem, int userId = 0) {
  if (!userId) {
    string studentName;
    listStudents(mySystem);
    cout << "Enter the student name" << endl;
    cin >> studentName;
    userId = getIndexFromSystem(mySystem, "users", studentName);
    if (!userId) {
      cout << "User doesn't exist" << endl;
    }
  }
  listStudentObjects(mySystem, userId);
  string objectName;
  cout << "Enter the file name you want to modify" << endl;
  cin >> objectName;
  int objectId = getIndexFromSystem(mySystem, "objects", objectName);
  if (objectId < 0) {
    cout << "File doesn't exist" << endl;
    return;
  }
  struct user currentUser = mySystem -> myUsers.users[userId];
  cout << "Type 'quit' in a new line to exit" << endl;
  string line;
  ofstream myfile;
  string filePath = "student_materials/" + currentUser.userName + "_files/" + objectName + ".txt";
  myfile.open(filePath.c_str(), std::ios_base::app);
  do {
    getline(cin, line);
    if (line != "quit") {
      string encryptedLine = encryptMessage(line, currentUser.rsa_keys.private_key);
      myfile << encryptedLine << "\n";
    }
  }
  while (line != "quit");
  myfile.close();
}

bool courseOrTeacherFileExist(struct system * mySystem, string id, string type) {
  int currIndex = mySystem -> myObjects.currentObjectIndex;
  for (int i = 0; i < currIndex + 1; i++) {
    if (mySystem -> myObjects.objects[i].objectName == id && mySystem -> myObjects.objects[i].filetype == type) {
      return true;
    }
  }
  return false;
}

void studentReadObject(struct system * mySystem, int userId = 0) {
  if (!userId) {
    string studentName;
    listStudents(mySystem);
    cout << "Enter the student name" << endl;
    cin >> studentName;
    userId = getIndexFromSystem(mySystem, "users", studentName);
    if (!userId) {
      cout << "User doesn't exist" << endl;
    }
  }
  listStudentObjects(mySystem, userId);
  string objectName;
  cout << "Enter the file name you want to view" << endl;
  cin >> objectName;
  int objectId = getIndexFromSystem(mySystem, "objects", objectName);
  if (objectId < 0) {
    cout << "File doesn't exist" << endl;
    return;
  }
  struct user currentUser = mySystem -> myUsers.users[userId];
  cout << "Type 'quit' in a new line to exit" << endl;
  string line;
  ifstream myfile;
  string filePath = "student_materials/" + currentUser.userName + "_files/" + objectName + ".txt";
  myfile.open(filePath.c_str());
  cout << "............................................................................." << endl;
  cout << "Contents of the File: " << endl;
  while (getline(myfile, line)) {
    string decryptedLine = decryptMessage(line, currentUser.rsa_keys.private_key);
    cout << decryptedLine << endl;
  }
  cout << endl;
  cout << "............................................................................." << endl;
  myfile.close();
}

// List All Available Students.
void listStudents(struct system * mySystem) {
  int currIndex = mySystem -> myUsers.currentUserIndex;
  int i = 0;
  cout << "............................................................................." << endl;
  if (currIndex > 0) {
    cout << "No \t Student Name" << endl;
    for (i = 0; i < currIndex + 1; i++) {
      if (mySystem -> myUsers.users[i].isStudent) {
        cout << i + 1 << "\t " << mySystem -> myUsers.users[i].userName << endl;
      }
    }
  } else {
    cout << "No Students Available" << endl;
  }
  cout << "............................................................................." << endl;
  cout << endl;
}

// List All Available Teachers.
void listTeachers(struct system * mySystem) {
  int currIndex = mySystem -> myUsers.currentUserIndex;
  int i = 0;
  cout << "............................................................................." << endl;
  if (currIndex > 0) {
    cout << "No \t Teacher Name" << endl;
    for (i = 0; i < currIndex + 1; i++) {
      if (mySystem -> myUsers.users[i].isTeacher) {
        cout << i + 1 << "\t " << mySystem -> myUsers.users[i].userName << endl;
      }
    }
  } else {
    cout << "No Teachers Available" << endl;
  }
  cout << "............................................................................." << endl;
  cout << endl;
}

void addTeacherObject(struct system * mySystem, string fileLocation) {
  int newIndex;
  string filePath1, filePath2, fileName;
  struct user currentUser;
  if (fileLocation == "course_materials") {
    cout << "Enter the file name of the object you want to create" << endl;
    cin >> fileName;
    currentUser = getCurrentUserDetails(mySystem);
    if (courseOrTeacherFileExist(mySystem, fileName, "course")) {
      cout << "File already exists" << endl;
      addTeacherObject(mySystem, fileLocation);
      return;
    }
    newIndex = ++mySystem -> myObjects.currentObjectIndex;
    filePath1 = "course_materials/" + fileName;
    filePath2 = "course_materials/" + fileName + ".txt";
    ofstream file(fileName[fileName.length() - 3] != '.' ? filePath2.c_str() : filePath1.c_str());
    cout << "The file was created succesfully" << endl;
    mySystem -> myObjects.objects[newIndex].objectName = fileName;
    mySystem -> myObjects.objects[newIndex].objectId = gen_id();
    mySystem -> myObjects.objects[newIndex].objectOwner = currentUser;
    mySystem -> myObjects.objects[newIndex].currentPermission = "777";
    mySystem -> myObjects.objects[newIndex].filetype = "course";
  } else if (fileLocation == "student_materials") {
    listStudents(mySystem);
    string studentName;
    cout << "Enter the student Name " << endl;
    cin >> studentName;
    int studentId = getIndexFromSystem(mySystem, "users", studentName);
    if (studentId < 0) {
      cout << "Student doesn't exist" << endl;
      return;
    }
    addStudentObject(mySystem, studentId);
  } else {
    cout << "Enter the file name of the object you want to create" << endl;
    cin >> fileName;
    currentUser = getCurrentUserDetails(mySystem);
    if (courseOrTeacherFileExist(mySystem, fileName, "teacher")) {
      cout << "File already exists" << endl;
      addTeacherObject(mySystem, fileLocation);
      return;
    }
    newIndex = ++mySystem -> myObjects.currentObjectIndex;
    filePath1 = "teacher_materials/" + fileName;
    filePath2 = "teacher_materials/" + fileName + ".txt";
    ofstream file(fileName[fileName.length() - 3] != '.' ? filePath2.c_str() : filePath1.c_str());
    cout << "The file was created succesfully" << endl;
    mySystem -> myObjects.objects[newIndex].objectName = fileName;
    mySystem -> myObjects.objects[newIndex].objectId = gen_id();
    mySystem -> myObjects.objects[newIndex].objectOwner = currentUser;
    mySystem -> myObjects.objects[newIndex].filetype = "teacher";
    mySystem -> myObjects.objects[newIndex].currentPermission = "777";
  }
}

void logout(struct system * mySystem) {
  mySystem -> authUserId = -1;
  cout << "............................................................................." << endl;
  cout << "logout successfull" << endl;
  cout << "............................................................................." << endl;
}

void displayAllObjects(struct system * mySystem) {
  int currIndex = mySystem -> myObjects.currentObjectIndex;
  int i = 0;
  cout << "............................................................................." << endl;
  if (currIndex >= 0) {
    cout << "No \t Object Name \t Object Owner " << endl;
    for (i = 0; i < currIndex + 1; i++) {
      string objectName = mySystem -> myObjects.objects[i].objectName;
      string objectOwner = mySystem -> myObjects.objects[i].objectOwner.userName;
      cout << i + 1 << "\t " << objectName << "\t      \t" << objectOwner << endl;
    }
  } else {
    cout << "No currents objects" << endl;
  }
  cout << "............................................................................." << endl;
  cout << endl;
}

struct system mySystem = createSystem();
struct system * mySystemPointer = & mySystem;
string encryptMessage(string message, int key) {
  	char ch;
	for(int i = 0; message[i] != '\0'; ++i){
		ch = message[i];
		if(!isalpha(message[i])){
			continue;
		}
		if(isspace(message[i])){
			message[i] = ' ';
		}
		else if(ch >= 'a' && ch <= 'z'){
			message[i] = (((message[i] - 97) + key) % 26) + 97;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			message[i] = (((message[i] - 65) + key) % 26) + 65;
		}
	}
  // Return the resulting string 
  return message;
}

string decryptMessage(string message, int key) {
	char ch;
  for(int i = 0; message[i] != '\0'; ++i){
  		ch = message[i];
		if(!isalpha(message[i])){
			continue;
		}
		else if(isspace(message[i])){
			message[i] = ' ';
		}
		else if(ch >= 'a' && ch <= 'z'){
			message[i] = ((( message[i] - (key - 85 ))  % 26) + 97);
		}
		else if(ch >= 'A' && ch <= 'Z'){
			message[i] = ((( message[i] - (key - 65)) % 26) + 65);
		}
	}

  // Return the resulting string 
  return message;
}

int main() {
//	for(char a='a'; a <= 'z'; a++){
//		string str(1,a);
//		string test = encryptMessage(str, 24);
//		string test2 = decryptMessage(test, 24);
//		cout<<"Original: "<<a<<" Encrypted: "<<test<<" Decrypted: "<<test2<<endl;
//	}
	
  cout << "\t\t\t\t\t## Welcome to the M.T. Class Notebook-like system ##" << endl;
  cout << "\nSystem Description:\n\t\t We build a simple M.T. Class Notebook like system,\n";
  cout << "where it have 3 types of actors, System Adminstrator, Teachers, and Students." << endl;
  cout << "\nCreate a new Default Account" << endl;
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
    
