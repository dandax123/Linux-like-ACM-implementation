#include <bits/stdc++.h>

#include <ctime>

#include <unistd.h>

#include <fstream>

#include <sstream>

using namespace std;

void addUserPermissionToObject(string permission, string currentUserName, string userId, int newIndex, struct system * mySystem);
void addGroupPermissionToObject(string permission, string groupName, string groupId, int objectIndex, struct system * mySystem);
void addNewUserPermssionToObject(string permission, string currentUserName, int newIndex, struct system * mySystem);
struct user getUserFromIdOrUserName(struct system * mySystem, string userNameOrId);
int getIndexOfGroupUser(struct system * mySystem, int groupIndex, string userName);
int getIndexFromSystem(struct system * mySystem, string type, string id);
void giveAllAdminsUserPermission(int newIndex, struct system * mySystem);
void listGroupUser(struct system * mySystem, int groupIndex);
struct myUsers getUsersNotInGroup(struct system * mySystem);
void setObjectPermission(struct system * mySystem);
void displayAllObjects(struct system * mySystem);
void displayAllGroups(struct system * mySystem);
string convertPermissionIntToString(int permit);
string convertPermissionIntToString(int permit);
void displayAllUsers(struct system * mySystem);

struct user {
  string userName;
  string password;
  int isAdmin;
  string userId;
  int isEnabled;
  bool isInGroup;
};

struct myUsers {
  struct user users[100];
  int currentUserIndex;
};

struct group {
  string groupId;
  string groupName;
  struct myUsers groupUsers;
  int isEnabled;
};

struct userPermission {
  string userId;
  string userName;
  int canRead;
  int canWrite;
  int canExecute;
};

struct groupPermission {
  string groupId;
  string groupName;

  int canRead;
  int canWrite;
  int canExecute;
};

struct userPermits {
  int currentUserPermission;
  struct userPermission currentUserPermissions[20];
};

struct groupPermits {
  int currentGroupPermission;
  struct groupPermission currentGroupPermissions[20];
};

struct object {
  string objectId;
  struct user objectOwner;
  string objectName;
  struct userPermits userPermission;
  struct groupPermits groupPermission;
  struct userPermission ownerPermission;
  string currentPermission;
};

struct myGroups {
  struct group groups[100];
  int currentGroupIndex;
};

struct myObjects {
  struct object objects[1000];
  int currentObjectIndex;
};

struct system {
  struct myUsers myUsers;
  struct myGroups myGroups;
  struct myObjects myObjects;
  int authUserId;
};

struct system createNewSystem() {
  struct system mySystem;
  mySystem.myUsers.currentUserIndex = -1;
  mySystem.myGroups.currentGroupIndex = -1;
  mySystem.myObjects.currentObjectIndex = -1;
  mySystem.authUserId = -1;
  if (mkdir("./files") != 0) {
    string directory = "files";
    string a = "rmdir /s /q " + directory;
    system(a.c_str());
    mkdir("./files");
  }
  return mySystem;
}

struct system mySystem = createNewSystem();
struct system * mySystemPointer = & mySystem;

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

bool isNumber(string s) {
  for (int i = 0; i < s.length(); i++)
    if (isdigit(s[i]) == false)
      return false;
  return true;
}

void addUserToSystem(struct system * mySystem, bool isDefaultAdmin = false) {
  string isAdmin = isDefaultAdmin ? "Y" : "";
  string userName;
  cout << "Enter the username: ";
  cin >> userName;
  int userExists = getIndexFromSystem(mySystem, "users", userName);
  if (userExists == -1) {
    int newIndex = ++mySystem -> myUsers.currentUserIndex;
    mySystem -> myUsers.users[newIndex].userName = userName;

    cout << "Enter  your password: ";
    cin >> mySystem -> myUsers.users[newIndex].password;
    if (!isDefaultAdmin) {
      cout << "Are you an admin ?(Y/n):  ";
      cin >> isAdmin;
    }

    mySystem -> myUsers.users[newIndex].isAdmin = isAdmin == "Y" ? 1 : 0;
    mySystem -> myUsers.users[newIndex].isEnabled = 1;
    mySystem -> myUsers.users[newIndex].isInGroup = false;
    mySystem -> myUsers.users[newIndex].userId = gen_id();
    cout << "User added successfully" << endl;
    setObjectPermission(mySystem);
  } else {
    cout << "User Already exists" << endl;
    cout << "Change the user name" << endl;
    cout << endl;
    addUserToSystem(mySystem);
  }
}

void addGroupToSystem(struct system * mySystem) {
  string groupName;
  cout << "Enter the group name" << endl;
  cin >> groupName;
  int groupExists = getIndexFromSystem(mySystem, "groups", groupName);
  if (groupExists == -1) {
    int newIndex = ++mySystem -> myGroups.currentGroupIndex;
    mySystem -> myGroups.groups[newIndex].groupName = groupName;
    mySystem -> myGroups.groups[newIndex].isEnabled = 1;
    mySystem -> myGroups.groups[newIndex].groupId = gen_id();
    mySystem -> myGroups.groups[newIndex].groupUsers.currentUserIndex = -1;
    setObjectPermission(mySystem);
    cout << "Succesfully added!" << endl;
  } else {
    cout << "Group Already exists" << endl;
    cout << "Change the group name" << endl;
    cout << endl;
    addGroupToSystem(mySystem);
  }
}

void displayAvailableUsers(struct system * mySystem, int groupIndex) {
  int currIndex = mySystem -> myUsers.currentUserIndex;
  int groupUserIndex = mySystem -> myGroups.groups[groupIndex].groupUsers.currentUserIndex;
  struct user freeUsers[20];
  int x = -1;
  for (int i = 0; i < currIndex + 1; i++) {
    if (i == mySystem -> authUserId) continue;
    bool isAmong = false;
    for (int j = 0; j < groupUserIndex + 1; j++) {
      if (mySystem -> myUsers.users[i].userId == mySystem -> myGroups.groups[groupIndex].groupUsers.users[j].userId) isAmong = true;
    }
    if (!isAmong) {
      freeUsers[++x] = mySystem -> myUsers.users[i];
    }
  }
  cout << "............................................................................." << endl;
  if (x >= 0) {
    cout << "Id \t Users \t Account Type \t Account Status" << endl;
    for (int i = 0; i < x + 1; i++) {
      string accountType = freeUsers[i].isAdmin == 1 ? "Admin" : "User";
      string accountStatus = freeUsers[i].isEnabled == 1 ? "Active" : "InActive";
      cout << i + 1 << "\t " << freeUsers[i].userName << "\t " << accountType << "\t \t  " << accountStatus << endl;
    }
  } else {
    cout << "No Available users" << endl;
  }
  cout << "............................................................................." << endl;

}

void addUserToGroup(struct system * mySystem, int groupIndex) {
  int currIndex = mySystem -> myUsers.currentUserIndex;
  if (currIndex > 0) {
    cout << "List of available Users" << endl;
    displayAvailableUsers(mySystemPointer, groupIndex);
    string userName;
    cout << "Enter  the username of the user" << endl;
    cin >> userName;
    struct user newUser = getUserFromIdOrUserName(mySystemPointer, userName);
    if (newUser.isEnabled) {
      int newIndex = ++mySystem -> myGroups.groups[groupIndex].groupUsers.currentUserIndex;
      mySystem -> myGroups.groups[groupIndex].groupUsers.users[newIndex] = newUser;
      //update isInGroup
      int userIndex = getIndexFromSystem(mySystem, "users", newUser.userName);
      mySystem -> myUsers.users[userIndex].isInGroup = true;
      cout << "Succesfully added!" << endl;
      setObjectPermission(mySystem);
      listGroupUser(mySystemPointer, groupIndex);
    } else {
      cout << "User doesn't exist or is disabled" << endl;
      return;
    }
  } else {
    cout << "No Possible user to add" << endl;
    return;
  }
}
void deleteUserFromGroup(struct system * mySystem, int groupIndex) {
  struct myUsers newSetofUsers;
  int allIndex = mySystem -> myGroups.groups[groupIndex].groupUsers.currentUserIndex;
  if (allIndex >= 0) {
    string userName;
    cout << "Enter the username to delete" << endl;
    cin >> userName;
    int userIndex = getIndexFromSystem(mySystem, "users", userName);
    mySystem -> myUsers.users[userIndex].isInGroup = false;
    int index = getIndexOfGroupUser(mySystemPointer, groupIndex, userName);
    for (int i = 0; i < index; i++) {
      newSetofUsers.users[i] = mySystem -> myGroups.groups[groupIndex].groupUsers.users[i];
    }
    for (int i = index; i < allIndex + 1; i++) {
      newSetofUsers.users[i] = mySystem -> myGroups.groups[groupIndex].groupUsers.users[i + 1];
    }
    newSetofUsers.currentUserIndex = mySystem -> myGroups.groups[groupIndex].groupUsers.currentUserIndex - 1;
    mySystem -> myGroups.groups[groupIndex].groupUsers = newSetofUsers;
    cout << "Deleted user sucessfully" << endl;
    setObjectPermission(mySystem);
  }
  return;
}

int getIndexOfGroupUser(struct system * mySystem, int groupIndex, string userName) {
  int currIndex = mySystem -> myGroups.groups[groupIndex].groupUsers.currentUserIndex;
  for (int i = 0; i < currIndex + 1; i++) {
    if (mySystem -> myGroups.groups[groupIndex].groupUsers.users[i].userName == userName) {
      return i;
    }
  }
}
int getIndexFromSystem(struct system * mySystem, string type, string id) {
  int index = -1;
  if (type == "groups") {
    int currIndex = mySystem -> myGroups.currentGroupIndex;
    for (int i = 0; i < currIndex + 1; i++) {
      if (mySystem -> myGroups.groups[i].groupName == id) {
        index = i;
      }
    }
  } else if (type == "users") {
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

void deleteFromSystem(struct system * mySystem, string type) {
  string userName;
  string operation = type == "groups" ? "group" : "user";
  if (type == "groups") {
    struct myGroups newSetofGroups;
    int allIndex = mySystem -> myGroups.currentGroupIndex;
    if (allIndex < 0) {
      return;
    }
    cout << "Enter the " << operation << " name you want to delete" << endl;
    cin >> userName;
    int index = getIndexFromSystem(mySystemPointer, "groups", userName);
    for (int i = 0; i < index; i++) {
      newSetofGroups.groups[i] = mySystem -> myGroups.groups[i];
    }
    for (int i = index; i < allIndex + 1; i++) {
      newSetofGroups.groups[i] = mySystem -> myGroups.groups[i + 1];
    }
    newSetofGroups.currentGroupIndex = mySystem -> myGroups.currentGroupIndex - 1;
    mySystem -> myGroups = newSetofGroups;
    cout << "Deleted group sucessfully" << endl;
    displayAllGroups(mySystemPointer);
  } else if (type == "users") {
    struct myUsers newSetofUsers;
    int allIndex = mySystem -> myUsers.currentUserIndex;
    if (allIndex < 1) {
      return;
    }
    cout << "Enter the " << operation << " name you want to delete" << endl;
    cin >> userName;
    int index = getIndexFromSystem(mySystemPointer, "users", userName);
    for (int i = 0; i < index; i++) {
      newSetofUsers.users[i] = mySystem -> myUsers.users[i];
    }
    for (int i = index; i < allIndex + 1; i++) {
      newSetofUsers.users[i] = mySystem -> myUsers.users[i + 1];
    }
    newSetofUsers.currentUserIndex = mySystem -> myUsers.currentUserIndex - 1;
    mySystem -> myUsers = newSetofUsers;
    cout << "Deleted user sucessfully" << endl;
    displayAllUsers(mySystemPointer);
  } else if (type == "objects") {
    displayAllObjects(mySystem);
    struct myObjects newSetofObjects;
    int allIndex = mySystem -> myObjects.currentObjectIndex;
    if (allIndex < 0) {
      return;
    }
    cout << "Enter the " << operation << " name you want to delete" << endl;
    cin >> userName;
    int index = getIndexFromSystem(mySystemPointer, "objects", userName);
    for (int i = 0; i < index; i++) {
      newSetofObjects.objects[i] = mySystem -> myObjects.objects[i];
    }
    for (int i = index; i < allIndex + 1; i++) {
      newSetofObjects.objects[i] = mySystem -> myObjects.objects[i + 1];
    }
    newSetofObjects.currentObjectIndex = mySystem -> myObjects.currentObjectIndex - 1;
    mySystem -> myObjects = newSetofObjects;
    cout << "Deleted object sucessfully" << endl;
  }
  setObjectPermission(mySystem);
  return;
}

void enableOrDisableGroupOrUser(struct system * mySystem, string type, int operation) {
  int currGroupIndex = mySystem -> myGroups.currentGroupIndex;
  int currUserIndex = mySystem -> myUsers.currentUserIndex;
  if (type == "group" && currGroupIndex != -1) {
    string groupName;
    cout << "Enter the " << type << " Name" << endl;
    cin >> groupName;
    int index = getIndexFromSystem(mySystem, "groups", groupName);
    mySystem -> myGroups.groups[index].isEnabled = operation;
    displayAllGroups(mySystemPointer);
  } else if (type == "user" && currUserIndex > 0) {
    string userName;
    cout << "Enter the user Name" << endl;
    cin >> userName;
    int index = getIndexFromSystem(mySystem, "users", userName);
    mySystem -> myUsers.users[index].isEnabled = operation;
    displayAllUsers(mySystemPointer);
  }

  if (currGroupIndex != -1 || currUserIndex > 0) {
    string result = operation ? "Enabled" : "Disabled";
    cout << "Successfully " << result << endl;
  } else {
    return;
  }
}

void setUserPermissions(struct system * mySystem, string objectName, string permission) {
  struct myUsers usersAdd = getUsersNotInGroup(mySystem);
  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);

  giveAllAdminsUserPermission(objectIndex, mySystem);
  if (usersAdd.currentUserIndex >= 0) {

    struct userPermits newPermit;
    mySystem -> myObjects.objects[objectIndex].userPermission = newPermit;
    mySystem -> myObjects.objects[objectIndex].userPermission.currentUserPermission = usersAdd.currentUserIndex;

    for (int i = 0; i < usersAdd.currentUserIndex + 1; i++) {
      struct user currentUser = usersAdd.users[i];
      if (!currentUser.isAdmin) {
        addUserPermissionToObject(permission, currentUser.userName, currentUser.userId, objectIndex, mySystem);
      }
    }
  }
}

void setOwnerPermissions(struct system * mySystem, string objectName, string permission) {
  struct userPermission newPermit;
  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);
  newPermit.userId = mySystem -> myObjects.objects[objectIndex].objectOwner.userId;
  newPermit.userName = mySystem -> myObjects.objects[objectIndex].objectOwner.userName;
  newPermit.canRead = permission[0] == '4' ? 1 : 0;
  newPermit.canWrite = permission[1] == '2' ? 1 : 0;
  newPermit.canExecute = permission[2] == '1' ? 1 : 0;
  mySystem -> myObjects.objects[objectIndex].ownerPermission = newPermit;
}

void setGroupPermissions(struct system * mySystem, string objectName, string permission) {
  int currGroupIndex = mySystem -> myGroups.currentGroupIndex;
  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);
  if (currGroupIndex == -1) {
    return;
  } else {
    struct groupPermits newPermit;
    mySystem -> myObjects.objects[objectIndex].groupPermission = newPermit;
    mySystem -> myObjects.objects[objectIndex].groupPermission.currentGroupPermission = -1;
    for (int i = 0; i < currGroupIndex + 1; i++) {
      struct group currentGroup = mySystem -> myGroups.groups[i];
      addGroupPermissionToObject(permission, currentGroup.groupName, currentGroup.groupId, objectIndex, mySystem);
    }
  }
}

void displayAllUsers(struct system * mySystem) {
  int currIndex = mySystem -> myUsers.currentUserIndex;
  int i = 0;
  cout << "............................................................................." << endl;
  if (currIndex > 0) {
    cout << "Id \t Users \t Account Type \t Account Status" << endl;
    for (i = 0; i < currIndex + 1; i++) {
      if (i == mySystem -> authUserId) continue;
      string accountType = mySystem -> myUsers.users[i].isAdmin == 1 ? "Admin" : "User";
      string accountStatus = mySystem -> myUsers.users[i].isEnabled == 1 ? "Active" : "InActive";
      cout << i + 1 << "\t " << mySystem -> myUsers.users[i].userName << "\t " << accountType << "\t \t  " << accountStatus << endl;
    }
  } else {
    cout << "No currents users" << endl;
  }
  cout << "............................................................................." << endl;
  cout << endl;
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
void displayAllGroups(struct system * mySystem) {
  int currIndex = mySystem -> myGroups.currentGroupIndex;
  int i = 0;
  cout << "............................................................................." << endl;
  if (currIndex == -1) {
    cout << "No currents Groups" << endl;
  } else {
    cout << "Id \t Groups \t Group Status" << endl;
    for (i = 0; i < currIndex + 1; i++) {
      string accountStatus = mySystem -> myGroups.groups[i].isEnabled == 1 ? "Active" : "InActive";
      cout << i + 1 << "\t " << mySystem -> myGroups.groups[i].groupName << "\t\t " << accountStatus << endl;
    }
  }
  cout << "............................................................................." << endl;
  cout << endl;
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
    mySystem -> myObjects.objects[newIndex].objectOwner = objectOwner;
    mySystem -> myObjects.objects[newIndex].currentPermission = "766";
    setOwnerPermissions(mySystem, fileName, convertPermissionIntToString(7));
    setGroupPermissions(mySystem, fileName, convertPermissionIntToString(6));
    setUserPermissions(mySystem, fileName, convertPermissionIntToString(6));
    cout << endl;
    displayAllObjects(mySystem);
  } else {
    cout << "File Already exists" << endl;
    cout << "Change the file name" << endl;
    cout << endl;
    createNewObject(mySystem);
  }
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

string convertPermissionStringToReadable(int permit) {
  string ans = "---";
  if (permit - 4 >= 0) {
    ans[0] = 'r';
    permit = permit - 4;
  }
  if (permit - 2 >= 0) {
    ans[1] = 'w';
    permit = permit - 2;
  }
  if (permit - 1 >= 0) {
    ans[2] = 'x';
    permit = permit - 1;
  }
  return ans;
}

void giveAllAdminsUserPermission(int objectIndex, struct system * mySystem) {
  int currIndex = mySystem -> myUsers.currentUserIndex;
  int i = 0;
  for (i = 0; i < currIndex; i++) {
    if (mySystem -> myUsers.users[i].isAdmin) {
      addUserPermissionToObject(convertPermissionIntToString(7), mySystem -> myUsers.users[i].userName, mySystem -> myUsers.users[i].userId, objectIndex, mySystemPointer);
    }
  }
}

void addUserPermissionToObject(string permission, string currentUserName, string userId, int objectIndex, struct system * mySystem) {
  int newIndex = objectIndex;
  mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermission = mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermission >= 0 ? mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermission : -1;
  int newPermissionIndex = ++mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermission;
  mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canRead = permission[0] == '4' ? 1 : 0;
  mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canWrite = permission[1] == '2' ? 1 : 0;
  mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canExecute = permission[2] == '1' ? 1 : 0;
  mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].userName = currentUserName;
  mySystem -> myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].userId = userId;
}

void addGroupPermissionToObject(string permission, string groupName, string groupId, int objectIndex, struct system * mySystem) {
  int newIndex = objectIndex;
  mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermission = mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermission >= 0 ? mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermission : -1;
  int newPermissionIndex = ++mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermission;
  mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermissions[newPermissionIndex].canRead = permission[0] == '4' ? 1 : 0;
  mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermissions[newPermissionIndex].canWrite = permission[1] == '2' ? 1 : 0;
  mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermissions[newPermissionIndex].canExecute = permission[2] == '1' ? 1 : 0;
  mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermissions[newPermissionIndex].groupName = groupName;
  mySystem -> myObjects.objects[newIndex].groupPermission.currentGroupPermissions[newPermissionIndex].groupId = groupId;
}

struct myUsers getUsersNotInGroup(struct system * mySystem) {
  int currIndex = mySystem -> myUsers.currentUserIndex;
  struct myUsers noGroup;
  int x = -1;
  if (currIndex >= 0) {
    for (int i = 0; i < currIndex + 1; i++) {
      if (mySystem -> myUsers.users[i].isInGroup == false && !mySystem -> myUsers.users[i].isAdmin) {
        noGroup.users[++x] = mySystem -> myUsers.users[i];
      }
    }
  }
  noGroup.currentUserIndex = x;
  return noGroup;
}

void displayUserDetails(struct system * mySystem, string userName) {
  int userId = getIndexFromSystem(mySystem, "users", userName);
  cout << "............................................................................." << endl;
  cout << "User Name: " << mySystem -> myUsers.users[userId].userName << endl;
  string accountType = mySystem -> myUsers.users[userId].isAdmin == 1 ? "Admin" : "User";
  string accountStatus = mySystem -> myUsers.users[userId].isEnabled == 1 ? "Active" : "InActive";
  cout << "User Account type: " << accountType << endl;
  cout << "User Status: " << accountStatus << endl;
  cout << "............................................................................." << endl;
}

struct user getCurrentUserDetails(struct system * mySystem) {
  if (mySystem -> authUserId >= 0) {
    return mySystem -> myUsers.users[mySystem -> authUserId];
  } else {
    struct user fakeUser;
    fakeUser.isEnabled = 0;
    fakeUser.isAdmin = 0;
    return fakeUser;
  }
}
struct user getUserFromIdOrUserName(struct system * mySystem, string userNameOrId) {
  int index = getIndexFromSystem(mySystemPointer, "users", userNameOrId);
  return mySystem -> myUsers.users[index];
}

void listGroupUser(struct system * mySystem, int groupIndex) {
  int groupUserIndex = mySystem -> myGroups.groups[groupIndex].groupUsers.currentUserIndex;
  if (groupUserIndex >= 0) {
    cout << "Users in the group" << endl;
    cout << "............................................................................." << endl;
    cout << "Users \t Account Type \t Account Status" << endl;
    for (int i = 0; i < groupUserIndex + 1; i++) {
      string accountType = mySystem -> myGroups.groups[groupIndex].groupUsers.users[i].isAdmin == 1 ? "Admin" : "User";
      string accountStatus = mySystem -> myGroups.groups[groupIndex].groupUsers.users[i].isEnabled == 1 ? "Active" : "InActive";
      cout << mySystem -> myGroups.groups[groupIndex].groupUsers.users[i].userName << "\t " << accountType << "\t \t  " << accountStatus << endl;
    }
    cout << "............................................................................." << endl;
  } else {
    cout << "............................................................................." << endl;
    cout << "No users in the group" << endl;
    cout << "............................................................................." << endl;
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
    displayUserDetails(mySystemPointer, userName);
  }
}

void logout(struct system * mySystem) {
  mySystem -> authUserId = -1;
  cout << "............................................................................." << endl;
  cout << "logout successfull" << endl;
  cout << "............................................................................." << endl;
}

bool isUserInGroup(struct system * mySystem, int groupIndex) {
  int groupUserIndex = mySystem -> myGroups.groups[groupIndex].groupUsers.currentUserIndex;
  struct user currentUser = getCurrentUserDetails(mySystem);
  for (int i = 0; i < groupUserIndex + 1; i++) {
    if (currentUser.userId == mySystem -> myGroups.groups[groupIndex].groupUsers.users[i].userId) {
      return true;
    }
  }
  return false;
}

struct groupPermission getGroupPermissionForObject(struct system * mySystem, int groupIndex, string objectName) {
  struct groupPermission isAccessible;
  isAccessible.canExecute = 0;
  isAccessible.canWrite = 0;
  isAccessible.canRead = 0;
  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);
  int currGroupPermissionIndex = mySystem -> myObjects.objects[objectIndex].groupPermission.currentGroupPermission >= 0 ? mySystem -> myObjects.objects[objectIndex].groupPermission.currentGroupPermission : -1;
  if (currGroupPermissionIndex >= 0) {
    for (int i = 0; i < currGroupPermissionIndex + 1; i++) {
      struct groupPermission objectGroupPermission = mySystem -> myObjects.objects[objectIndex].groupPermission.currentGroupPermissions[i];
      if (mySystem -> myGroups.groups[groupIndex].groupId == objectGroupPermission.groupId) {
        isAccessible.canExecute = max(isAccessible.canExecute, objectGroupPermission.canExecute);
        isAccessible.canRead = max(isAccessible.canExecute, objectGroupPermission.canRead);
        isAccessible.canWrite = max(isAccessible.canExecute, objectGroupPermission.canWrite);
      }
    }
  }
  return isAccessible;
}

struct userPermission groupPermissionsForObject(struct system * mySystem, string objectName) {
  struct userPermission isAccessible;
  isAccessible.canExecute = 0;
  isAccessible.canWrite = 0;
  isAccessible.canRead = 0;
  int allCurGroupIndex = mySystem -> myGroups.currentGroupIndex;
  for (int i = 0; i < allCurGroupIndex + 1; i++) {
    int groupUserIndex = mySystem -> myGroups.groups[i].groupUsers.currentUserIndex;
    if (groupUserIndex >= 0) {
      if (isUserInGroup(mySystem, i)) {
        struct groupPermission object_i = getGroupPermissionForObject(mySystem, i, objectName);
        isAccessible.canExecute = max(isAccessible.canExecute, object_i.canExecute);
        isAccessible.canRead = max(isAccessible.canExecute, object_i.canRead);
        isAccessible.canWrite = max(isAccessible.canExecute, object_i.canWrite);
      }
    } else continue;
  }
  return isAccessible;
}

struct userPermission maxPermissionForObjectUser(struct system * mySystem, string objectName) {
  struct userPermission isAccessible;
  isAccessible.canExecute = 0;
  isAccessible.canWrite = 0;
  isAccessible.canRead = 0;
  struct user currentUser = getCurrentUserDetails(mySystem);
  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);
  int currUserPermissionIndex = mySystem -> myObjects.objects[objectIndex].userPermission.currentUserPermission >= 0 ? mySystem -> myObjects.objects[objectIndex].userPermission.currentUserPermission : -1;
  if (currUserPermissionIndex >= 0) {
    for (int i = 0; i < currUserPermissionIndex + 1; i++) {
      struct userPermission objectUserPermission = mySystem -> myObjects.objects[objectIndex].userPermission.currentUserPermissions[i];
      if (currentUser.userId == objectUserPermission.userId) {
        isAccessible.canExecute = max(isAccessible.canExecute, objectUserPermission.canExecute);
        isAccessible.canRead = max(isAccessible.canExecute, objectUserPermission.canRead);
        isAccessible.canWrite = max(isAccessible.canExecute, objectUserPermission.canWrite);
      }
    }
  }
  return isAccessible;
}

struct userPermission canAccessthisFile(struct system * mySystem, string objectName) {
  // initialize default permissions
  struct userPermission isAccessible;
  isAccessible.canExecute = 0;
  isAccessible.canWrite = 0;
  isAccessible.canRead = 0;
  struct user currentUser = getCurrentUserDetails(mySystem);
  isAccessible.userId = currentUser.userId;
  isAccessible.userName = currentUser.userName;
  // max permissions for any group that the user and the object belongs too
  if (currentUser.isInGroup) {
    struct userPermission maxGroupPermission = groupPermissionsForObject(mySystem, objectName);
    isAccessible.canExecute = max(isAccessible.canExecute, maxGroupPermission.canExecute);
    isAccessible.canRead = max(isAccessible.canExecute, maxGroupPermission.canRead);
    isAccessible.canWrite = max(isAccessible.canExecute, maxGroupPermission.canWrite);
  }
  //max permissions for users of an object
  struct userPermission maxUserPermission = maxPermissionForObjectUser(mySystem, objectName);
  isAccessible.canExecute = max(isAccessible.canExecute, maxUserPermission.canExecute);
  isAccessible.canRead = max(isAccessible.canExecute, maxUserPermission.canRead);
  isAccessible.canWrite = max(isAccessible.canExecute, maxUserPermission.canWrite);
  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);
  // if the current user is the owner of the file
  if (currentUser.userId == mySystem -> myObjects.objects[objectIndex].ownerPermission.userId) {
    isAccessible.canExecute = max(isAccessible.canExecute, mySystem -> myObjects.objects[objectIndex].ownerPermission.canExecute);
    isAccessible.canRead = max(isAccessible.canExecute, mySystem -> myObjects.objects[objectIndex].ownerPermission.canRead);
    isAccessible.canWrite = max(isAccessible.canExecute, mySystem -> myObjects.objects[objectIndex].ownerPermission.canWrite);
  }
  return isAccessible;
}

void readObject(struct system * mySystem) {
  displayAllObjects(mySystem);
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

void setPermission(struct system * mySystem) {
  displayAllObjects(mySystem);
  string objectName;
  cout << "Enter the name of the object you want to set the permission" << endl;
  cin >> objectName;
  string permission = "000";
  cout << "Enter the permission: (0-777)" << endl;
  cin >> permission;
  for (int i = 0; i < 3; i++) {
    if (permission.substr(i, 1) == "") permission[i] = '0';
  }
  setUserPermissions(mySystem, objectName, convertPermissionIntToString(permission[2] - '0'));
  setGroupPermissions(mySystem, objectName, convertPermissionIntToString(permission[1] - '0'));
  setOwnerPermissions(mySystem, objectName, convertPermissionIntToString(permission[0] - '0'));
  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);
  mySystem -> myObjects.objects[objectIndex].currentPermission = permission;
  cout << "New Permission Set for the file" << endl;
}

void getPermission(struct system * mySystem) {
  displayAllObjects(mySystem);
  string objectName;
  cout << "Enter the name of the object you want to get the permission" << endl;
  cin >> objectName;
  int objectIndex = getIndexFromSystem(mySystem, "objects", objectName);
  string currentPermission = mySystem -> myObjects.objects[objectIndex].currentPermission;
  cout << "............................................................................." << endl;
  cout << "Permissions for the file:" << endl;
  cout << "Owner: " << convertPermissionStringToReadable(currentPermission[0] - '0') << endl;
  cout << "Group: " << convertPermissionStringToReadable(currentPermission[1] - '0') << endl;
  cout << "Others: " << convertPermissionStringToReadable(currentPermission[2] - '0') << endl;
  cout << "............................................................................." << endl;
}

void setObjectPermission(struct system * mySystem) {
  int currIndex = mySystem -> myObjects.currentObjectIndex;
  int i = 0;

  if (currIndex >= 0) {
    for (i = 0; i < currIndex + 1; i++) {
      string objectName = mySystem -> myObjects.objects[i].objectName;
      int currentOwnerPermission = (mySystem -> myObjects.objects[i].currentPermission[0] - '0');
      int currentGroupPermission = (mySystem -> myObjects.objects[i].currentPermission[1] - '0');
      int currentUserPermission = (mySystem -> myObjects.objects[i].currentPermission[2] - '0');
      setOwnerPermissions(mySystem, objectName, convertPermissionIntToString(currentOwnerPermission));
      if (currentGroupPermission > 0) {
        setGroupPermissions(mySystem, objectName, convertPermissionIntToString(currentGroupPermission));
      }
      if (currentUserPermission > 0) {
        setUserPermissions(mySystem, objectName, convertPermissionIntToString(currentUserPermission));
      }
    }
  }
}

int main() {
  cout << "\t\t\t\t\t## Welcome to the Linux-like system ##" << endl;
  cout << endl;
  cout << "Create a new Default Account" << endl;

  addUserToSystem(mySystemPointer, true); // Adding an admin user 

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
          cout << "1. Add new user/group" << endl;
          cout << "2. Disable/Enable a user/group" << endl;
          cout << "3. Add/Delete user to/from group" << endl;
          cout << "4. Delete Group/user" << endl;
          cout << "5. Objects management" << endl;
          cout << "6. Access control management" << endl;
          cout << "7. List Users/Groups/Objects" << endl;
          cout << "8. Display User Detail" << endl;
          cout << "0. Return" << endl;
          cin >> action2;

          if (action2 == 1) // => Add new user/group.
          {
            int actionUserAdd = 1;
            while (actionUserAdd) {
              cout << "1. Add User" << endl;
              cout << "2. Add Group" << endl;
              cout << "0. Return" << endl;
              cin >> actionUserAdd;

              if (actionUserAdd == 2) {
                addGroupToSystem(mySystemPointer);
              } else if (actionUserAdd == 1) {
                addUserToSystem(mySystemPointer);
              } else break;
            }
            system("CLS"); // Clearing screen.
          }

          if (action2 == 2) // => Disable/Enable a user or group.
          {
            int actionDisable = 1;
            while (actionDisable) {
              cout << "1. Enable Group" << endl;
              cout << "2. Disable Group" << endl;
              cout << "3. Enable User" << endl;
              cout << "4. Disable User" << endl;
              cout << "0. Return" << endl;
              cin >> actionDisable;

              if (actionDisable == 1 || actionDisable == 2) {
                displayAllGroups(mySystemPointer);
                enableOrDisableGroupOrUser(mySystemPointer, "group", actionDisable == 1 ? 1 : 0);
              } else if (actionDisable == 3 || actionDisable == 4) {
                displayAllUsers(mySystemPointer);
                enableOrDisableGroupOrUser(mySystemPointer, "user", actionDisable >= 3 && actionDisable < 4 ? 1 : 0);
              } else break;
            }
            system("CLS"); //Screen Clearing.
          }
          if (action2 == 3) {
            displayAllGroups(mySystemPointer);
            string groupName;
            cout << "Enter the Group name you want to modify" << endl;
            cin >> groupName;
            int index = getIndexFromSystem(mySystemPointer, "groups", groupName);
            int actionGroupWork = 1;
            while (actionGroupWork) {
              listGroupUser(mySystemPointer, index);
              cout << "1. Add user to Group" << endl;
              cout << "2. Delete user from Group" << endl;
              cout << "0. Return" << endl;
              cin >> actionGroupWork;
              if (actionGroupWork == 1) {
                addUserToGroup(mySystemPointer, index);
              } else if (actionGroupWork == 2) {
                deleteUserFromGroup(mySystemPointer, index);
              } else break;
            }
            system("CLS");
          }
          if (action2 == 4) {
            int actionDelete = 1;
            while (actionDelete) {
              cout << "1. Delete Group" << endl;
              cout << "2. Delete Users" << endl;
              cout << "0. Return" << endl;
              cin >> actionDelete;
              if (actionDelete == 1) {
                displayAllGroups(mySystemPointer);
                deleteFromSystem(mySystemPointer, "groups");
              } else if (actionDelete == 2) {
                displayAllUsers(mySystemPointer);
                deleteFromSystem(mySystemPointer, "users");
              } else break;
            }
            system("CLS"); // Clearing Screen.
          }
          if (action2 == 5) {
            int action3x = 1;
            while (action3x) {
              cout << "1. Create Object" << endl;
              cout << "2. Read Object" << endl;
              cout << "3. Write Object" << endl;
              cout << "4. Delete Object" << endl;
              cout << "0. Return" << endl;
              cin >> action3x;
              if (action3x == 1) {
                createNewObject(mySystemPointer);
              }
              if (action3x == 2) {
                readObject(mySystemPointer);
              }
              if (action3x == 3) {
                writeObject(mySystemPointer);
              }
              if (action3x == 4) {
                deleteFromSystem(mySystemPointer, "objects");
              } else break;
            }
          }
          if (action2 == 6) {
            int actionAccess = 1;
            while (actionAccess) {
              cout << "1. Get Permission" << endl;
              cout << "2. Set Permission" << endl;
              cout << "0. Return" << endl;
              cin >> actionAccess;
              if (actionAccess == 1) {
                getPermission(mySystemPointer);
              }
              if (actionAccess == 2) {
                setPermission(mySystemPointer);
              } else break;
            }
          }
          if (action2 == 7) {
            int actionList = 1;
            while (actionList) {
              cout << "1. List All Users" << endl;
              cout << "2. List All Groups" << endl;
              cout << "3. List All Objects" << endl;
              cout << "0. Return" << endl;
              cin >> actionList;
              if (actionList == 1) {
                displayAllUsers(mySystemPointer);
              } else if (actionList == 2) {
                displayAllGroups(mySystemPointer);
              } else if (actionList == 3) {
                displayAllObjects(mySystemPointer);
              } else break;
            }
          }
          if (action2 == 8) {
            int actionDisplay = 1;
            while (actionDisplay) {
              displayAllUsers(mySystemPointer);
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

      //user menu
      else {
        while (action2 > 0 && mySystem.authUserId >= 0) {
          cout << "1. Create Object" << endl;
          cout << "2. Read Object" << endl;
          cout << "3. Write Object" << endl;
          cout << "4. Delete Object" << endl;
          cout << "0. Return" << endl;
          cin >> action2;
          if (action2 == 1) {
            createNewObject(mySystemPointer);
          }
          if (action2 == 2) {
            readObject(mySystemPointer);
          }
          if (action2 == 3) {
            writeObject(mySystemPointer);
          }
          if (action2 == 4) {
            deleteFromSystem(mySystemPointer, "groups");
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
