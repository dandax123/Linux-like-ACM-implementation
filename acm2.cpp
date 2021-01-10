# include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
struct userPermission canAccessthisFile(struct system * mySystem, string objectName);
void displayUserDetails(struct system * mySystem, string userName);
void addUserToSystem(struct system * mySystem, string userType );
string convertPermissionIntToString(int permit);
void createNewObject(struct system * mySystem);
void displayAllObjects(struct system * mySystem);
void displayUserDetails(struct system * mySystem, string userName);
string gen_id();
struct user getCurrentUserDetails(struct system * mySystem);
int getIndexFromSystem(struct system * mySystem, string type, string id);
string getUserStatus(struct user user);
void readObject(struct system * mySystem);
void userLogin(struct system * mySystem);
void writeObject(struct system * mySystem);
void logout(struct system * mySystem);
struct rsa_key{
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
struct userPermission {
  string userId;
  string userName;
  int canRead;
  int canWrite;
};
struct objectPermissions {
  int currentPermission;
  struct userPermission objectPermissions[100];
};
struct myUsers {
  struct user users[100];
  int currentUserIndex;
};
struct object {
  string objectId;
  struct user objectOwner;
  string objectName;
  struct objectPermissions permissions;  
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
string getUserStatus(struct user user){
	if(user.isAdmin) return "Admin";
	if(user.isStudent) return "Student";
	if(user.isTeacher) return "Teacher";
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
    //displayUserDetails(mySystemPointer, userName);
  }
}

void addUserToSystem(struct system * mySystem, string userType) {
  string userName;
  cout << "Enter the username: ";
  cin >> userName;
  int userExists = getIndexFromSystem(mySystem, "users", userName);
  if (userExists == -1) {
    int newIndex = ++mySystem -> myUsers.currentUserIndex;
    mySystem -> myUsers.users[newIndex].userName = userName;
    cout << "Enter  your password: ";
    cin >> mySystem -> myUsers.users[newIndex].password;
    mySystem -> myUsers.users[newIndex].isAdmin = userType=="admin" ? 1: 0;
    mySystem -> myUsers.users[newIndex].isStudent = userType=="student" ? 1: 0;
    mySystem -> myUsers.users[newIndex].isTeacher = userType=="teacher" ? 1: 0;
    if(userType == "student"){
      string directory= "./student_materials/" + userName + "_files";
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

struct userPermission canAccessthisFile(struct system * mySystem, string objectName) {
  // initialize default permissions
  struct userPermission isAccessible;
  isAccessible.canWrite = 0;
  isAccessible.canRead = 0;
  struct user currentUser = getCurrentUserDetails(mySystem);
  isAccessible.userId = currentUser.userId;
  isAccessible.userName = currentUser.userName;

  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);
  // if the current user is the owner of the file
//  if (currentUser.userId == mySystem -> myObjects.objects[objectIndex].ownerPermission.userId) {
//    isAccessible.canRead = max(isAccessible.canExecute, mySystem -> myObjects.objects[objectIndex].ownerPermission.canRead);
//    isAccessible.canWrite = max(isAccessible.canExecute, mySystem -> myObjects.objects[objectIndex].ownerPermission.canWrite);
//  }
  return isAccessible;
}
string convertPermissionIntToString(int permit) {
  string ans = "000";
  if (permit - 4 >= 0) {
    ans[0] = '4';
    permit = permit - 4;
  }
  if (permit - 2 >= 0) {
    ans[1] = '2';
    permit = permit - 2;
  }
  if (permit - 1 >= 0) {
    ans[2] = '1';
    permit = permit - 1;
  }
  return ans;
}
void createNewObject(struct system * mySystem) {
  string fileName;
  cout << "Input the file name that you want to create" << endl;
  cin >> fileName;
  int fileExists = getIndexFromSystem(mySystem, "objects", fileName);
  if (fileExists == -1) {
    int newIndex = ++mySystem -> myObjects.currentObjectIndex;
    string filePath1 = "files/" + fileName;
    string filePath2 = "files/" + fileName + ".txt";
    ofstream file(fileName[fileName.length() - 3] != '.' ? filePath2.c_str() : filePath1.c_str());
    cout << "The file was created succesfully" << endl;
    struct user objectOwner = mySystem -> myUsers.users[mySystem -> authUserId];
    mySystem -> myObjects.objects[newIndex].objectName = fileName;
    mySystem -> myObjects.objects[newIndex].objectId = gen_id();
   // mySystem -> myObjects.objects[newIndex].objectOwner = objectOwner;
   // mySystem -> myObjects.objects[newIndex].currentPermission = "766";
    //setOwnerPermissions(mySystem, fileName, convertPermissionIntToString(7));
    //setGroupPermissions(mySystem, fileName, convertPermissionIntToString(6));
    //setUserPermissions(mySystem, fileName, convertPermissionIntToString(6));
    cout << endl;
    displayAllObjects(mySystem);
  } else {
    cout << "File Already exists" << endl;
    cout << "Change the file name" << endl;
    cout << endl;
    createNewObject(mySystem);
  }
}

void readObject(struct system * mySystem) {
  //displayAllObjects(mySystem);
  int currObjIndex = mySystem -> myObjects.currentObjectIndex;
  if (currObjIndex >= 0) {
    string objectName;
    cout << "Enter the name of the object you want to read" << endl;
    cin >> objectName;
    struct userPermission myObjectPermission = canAccessthisFile(mySystem, objectName);
    if (myObjectPermission.canRead) {
      string line;
      ifstream myfile;
      string filePath = "files/" + objectName + ".txt";
      myfile.open(filePath.c_str());
      cout << "............................................................................." << endl;
      cout << "Contents of the File: " << endl;
      while (getline(myfile, line)) {
        cout << line << endl;
      }
      cout << endl;
      cout << "............................................................................." << endl;
    } else {
      cout << "You don't have permissions to read this file " << endl;
    }
  }
  return;
}

void writeObject(struct system * mySystem) {
  displayAllObjects(mySystem);
  int currObjIndex = mySystem -> myObjects.currentObjectIndex;
  if (currObjIndex >= 0) {
    string objectName;
    cout << "Enter the name of the object you want to write to" << endl;
    cin >> objectName;
    struct userPermission myObjectPermission = canAccessthisFile(mySystem, objectName);
    if (myObjectPermission.canWrite) {
      cout << "Type 'quit' in a new line to exit" << endl;
      string line;
      ofstream myfile;
      string filePath = "files/" + objectName + ".txt";
      myfile.open(filePath.c_str());
      do {
        getline(cin, line);
        if (line != "quit") {
          myfile << line << "\n";
        }
      }
      while (line != "quit");
      myfile.close();
    } else {
      cout << "You don't have permission to write to this file" << endl;
    }
  }
  return;
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
struct system * mySystemPointer = &mySystem;


int main() {
  cout << "\t\t\t\t\t## Welcome to the Notebook  system ##" << endl;
  cout << endl;
  cout << "Create a new Default Account" << endl;
  addUserToSystem(mySystemPointer, "admin"); // Adding an admin user 
  system("CLS"); // Clearing the screen.

  int action1 = 1;
  while (action1 > 0) {
    if (mySystem.authUserId < 0) {
      cout << endl << "  1. Login" << endl;
    }
    if (mySystem.authUserId >= 0) {
      cout << "2. Logout" << endl;
    }
    cout << "  0. Quit" << endl;
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
          cout << "Admin Menu" << endl;
          cout << "1. Add new user" << endl;
          cout << "4. Delete a user" << endl;
          cout << "5. Objects management" << endl;
          cout << "8. Display User Detail" << endl;
          cout << "0. Return" << endl;
          cin >> action2;

          if (action2 == 1) // => Add new user/group.
          {
            int actionUserAdd = 1;
            while (actionUserAdd) {
              cout << "1. Add Student" << endl;
              cout << "2. Add Teacher" << endl;
              cout << "0. Return" << endl;
              cin >> actionUserAdd;

//              if (actionUserAdd == 2) {
//                addGroupToSystem(mySystemPointer);
//              } else if (actionUserAdd == 1) {
//                addUserToSystem(mySystemPointer);
//              } else break;
            }
            system("CLS"); // Clearing screen.
          }

          if (action2 == 4) {
            int actionDelete = 1;
            while (actionDelete) {
              cout << "2. Delete Users" << endl;
              cout << "0. Return" << endl;
              cin >> actionDelete;
              if (actionDelete == 1) {
                //displayAllGroups(mySystemPointer);
                //deleteFromSystem(mySystemPointer, "groups");
              } else if (actionDelete == 2) {
                //displayAllUsers(mySystemPointer);
                //deleteFromSystem(mySystemPointer, "users");
              } else break;
            }
            system("CLS"); // Clearing Screen.
          }
          
          if (action2 == 7) {
            int actionList = 1;
            while (actionList) {
              cout << "1. List All Students" << endl;
              cout << "2. List All Teachers" << endl;
              cout << "3. List All Materials" << endl;
              cout << "0. Return" << endl;
              cin >> actionList;
//              if (actionList == 1) {
//                displayAllUsers(mySystemPointer);
//              } else if (actionList == 2) {
//                displayAllGroups(mySystemPointer);
//              } else if (actionList == 3) {
//                displayAllObjects(mySystemPointer);
//              } else break;
            }
          }
          if (action2 == 8) {
            int actionDisplay = 1;
            while (actionDisplay) {
              //displayAllUsers(mySystemPointer);
              cout << "1. Display User Detail By Name" << endl;
              cout << "0. Return" << endl;
              cin >> actionDisplay;
              if (actionDisplay == 1) {
                int userList;
                string userName;
                cout << endl << "Enter the userName" << endl;
                cin >> userName;
                displayUserDetails(mySystemPointer, userName);

              } else break;
            }
          }
        }
      }

      //student menu
      else if(currentUser.isStudent && mySystem.authUserId >= 0) {
        while (action2 > 0 && mySystem.authUserId >= 0) {
          cout << "1. View Course Material " << endl;
          cout << "2. Manage Student Materials" << endl;
          cout << "0. Return" << endl;
          cin >> action2;
//          if (action2 == 1) {
//            createNewObject(mySystemPointer);
//          }
//          if (action2 == 2) {
//            readObject(mySystemPointer);
//          }
//          if (action2 == 3) {
//            writeObject(mySystemPointer);
//          }
//          if (action2 == 4) {
//            deleteFromSystem(mySystemPointer, "groups");
//          }
//        }
		}
      }
      //teacher menu
      else if(currentUser.isTeacher && mySystem.authUserId >= 0) {
        while (action2 > 0 && mySystem.authUserId >= 0) {
          cout << "1. Manage Course Materials" << endl;
          cout << "2. Manage Teacher Materials" << endl;
          cout <<"3. Manage Student Materials"<<endl;
		  cout << "0. Return" << endl;	
          cin >> action2;
          if (action2 == 1) {
          	int actionCourse = 1;
          	while(actionCourse){
          		cout<<"1. Create new Course material"<<endl;
          		cout<<"2. Write/Edit Course Material"<<endl;
          		cout<<"3. Read from Course Material"<<endl;
				cout<<"4. Delete Course Material"<<endl;
				cout << "0. Return" << endl;
			}
            createNewObject(mySystemPointer);
          }
          if (action2 == 2) {
            int actionTeacher =1;
            while(actionTeacher){
          		cout<<"1. Create new Teacher material"<<endl;
          		cout<<"2. Write/Edit Teacher Material"<<endl;
          		cout<<"3. Read from Teacher Material"<<endl;
				cout<<"4. Delete Teacher Material"<<endl;
				cout << "0. Return" << endl;
			}
          }
          if(action2 == 3){
          	int actionStudent = 1;
          	while(actionStudent){
          		cout<<"1. List Students"<<endl;
          		cout<<"2. Manage Student Material"<<endl;
          		cout << "0. Return" << endl;
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





