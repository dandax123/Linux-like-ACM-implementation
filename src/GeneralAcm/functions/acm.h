#ifndef _ACM_H
#define _ACM_H
#include <bits/stdc++.h>
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

struct system createNewSystem();

string gen_id();

bool isNumber(string s);

void addUserToSystem(struct system * mySystem, bool isDefaultAdmin);

void addGroupToSystem(struct system * mySystem);

void displayAvailableUsers(struct system * mySystem, int groupIndex);

void addUserToGroup(struct system * mySystem, int groupIndex);

void deleteUserFromGroup(struct system * mySystem, int groupIndex);

int getIndexOfGroupUser(struct system * mySystem, int groupIndex, string userName);

int getIndexFromSystem(struct system * mySystem, string type, string id);

void deleteFromSystem(struct system * mySystem, string type);

void enableOrDisableGroupOrUser(struct system * mySystem, string type, int operation);

void setUserPermissions(struct system * mySystem, string objectName, string permission);

void setOwnerPermissions(struct system * mySystem, string objectName, string permission);

void setGroupPermissions(struct system * mySystem, string objectName, string permission);

void displayAllUsers(struct system * mySystem);

void displayAllObjects(struct system * mySystem);

void displayAllGroups(struct system * mySystem);

void createNewObject(struct system * mySystem);

string convertPermissionIntToString(int permit);

string convertPermissionStringToReadable(int permit);

void giveAllAdminsUserPermission(int objectIndex, struct system * mySystem);

void addUserPermissionToObject(string permission, string currentUserName, string userId, int objectIndex, struct system * mySystem);

void addGroupPermissionToObject(string permission, string groupName, string groupId, int objectIndex, struct system * mySystem);

struct myUsers getUsersNotInGroup(struct system * mySystem);

void displayUserDetails(struct system * mySystem, string userName);

struct user getCurrentUserDetails(struct system * mySystem);

struct user getUserFromIdOrUserName(struct system * mySystem, string userNameOrId);

void listGroupUser(struct system * mySystem, int groupIndex);

void userLogin(struct system * mySystem);

void logout(struct system * mySystem);

bool isUserInGroup(struct system * mySystem, int groupIndex);

struct groupPermission getGroupPermissionForObject(struct system * mySystem, int groupIndex, string objectName);

struct userPermission groupPermissionsForObject(struct system * mySystem, string objectName);

struct userPermission maxPermissionForObjectUser(struct system * mySystem, string objectName);

struct userPermission canAccessthisFile(struct system * mySystem, string objectName);

void readObject(struct system * mySystem);

void writeObject(struct system * mySystem);

void setPermission(struct system * mySystem);

void getPermission(struct system * mySystem);

void setObjectPermission(struct system * mySystem);

#endif
