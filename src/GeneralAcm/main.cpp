#include <bits/stdc++.h>
#include "functions/acm.h"
using namespace std;
int main() {
  struct system mySystem = createNewSystem();
  struct system *mySystemPointer = &mySystem;
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
                addUserToSystem(mySystemPointer, false);
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
