#include <bits/stdc++.h>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <sstream>
using namespace std;

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

struct system createNewSystem(); 
