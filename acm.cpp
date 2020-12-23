#include <bits/stdc++.h>
#include <ctime>
#include <unistd.h>
#include <fstream> 

using namespace std;
void giveAllAdminsUserPermission(int newIndex, struct system *mySystem);
string convertPermissionIntToString(int permit);
void addNewUserPermssionToObject (string permission, string currentUserName, int newIndex, struct system *mySystem );
void displayAllUsers (struct system *mySystem);
void displayAllGroups (struct system *mySystem);
struct user getUserFromIdOrUserName (struct system *mySystem, string userNameOrId);
void listGroupUser (struct system *mySystem, int groupIndex);
struct user {
	string userName;
	string password;
	int isAdmin;
	string userId;
	int isEnabled;
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
	string objectOwner;
	string objectName;
	struct userPermits userPermission;
	struct groupPermits groupPermission;
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

struct system createNewSystem(){
	struct system mySystem;
	mySystem.myUsers.currentUserIndex = -1;
	mySystem.myGroups.currentGroupIndex = -1;
	mySystem.myObjects.currentObjectIndex = -1;
	mySystem.authUserId = -1;
	return mySystem;
}
struct system mySystem = createNewSystem();
struct system *mySystemPointer = &mySystem;
string gen_id() {
    const int len = 30;
    string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    srand( (unsigned) time(NULL) * getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) 
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    
    
    return tmp_s;
    
}
bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
 
    return true;
}

void addUserToSystem (struct system *mySystem) {
	
	int newIndex = ++mySystem->myUsers.currentUserIndex;
	string isAdmin;
	cout<<"Enter the username: ";
	cin>>mySystem->myUsers.users[newIndex].userName;
	cout<<"Enter  your password: ";
	cin>>mySystem->myUsers.users[newIndex].password;
	cout<<"Are you an admin ?(Y/n):  ";
	cin>>isAdmin;
	mySystem->myUsers.users[newIndex].isAdmin = isAdmin == "Y" ? 1 : 0;
	mySystem->myUsers.users[newIndex].isEnabled = 1;
	mySystem->myUsers.users[newIndex].userId = gen_id();
	cout<<"User added successfully"<<endl;
	cout<<endl;
}

void addGroupToSystem(struct system *mySystem){
	
	int newIndex = ++mySystem->myGroups.currentGroupIndex;
	cout<<"Enter the group name"<<endl;
	cin>>mySystem->myGroups.groups[newIndex].groupName;
	mySystem->myGroups.groups[newIndex].isEnabled = 1;
	mySystem->myGroups.groups[newIndex].groupId = gen_id();
	mySystem->myGroups.groups[newIndex].groupUsers.currentUserIndex = -1;
	cout<<"Succesfully added!"<<endl;
}
void displayAvailableUsers  (struct system *mySystem, int groupIndex){
	int currIndex = mySystem->myUsers.currentUserIndex;
	int groupUserIndex = mySystem->myGroups.groups[groupIndex].groupUsers.currentUserIndex;
	cout<< groupUserIndex<<endl;
	struct user freeUsers[20];
	int x = -1;
	for(int i=0; i<currIndex + 1; i++){
		if(i == mySystem->authUserId) continue;
		bool isAmong = false;
		for(int j = 0; j< groupUserIndex+1; j++){
//			cout<<mySystem->myUsers.users[i].userId <<" "<<mySystem->myGroups.groups[groupIndex].groupUsers.users[j].userId<<endl;
			if(mySystem->myUsers.users[i].userId == mySystem->myGroups.groups[groupIndex].groupUsers.users[j].userId) isAmong = true;
		}
		if(!isAmong){
			freeUsers[++x] = mySystem->myUsers.users[i];
		}
	}
	cout<<"............................................................................."<<endl;
	if(x > 0 ){
		cout<<"Id \t Users \t Account Type \t Account Status"<<endl;	
		for(int i=0; i< x+1; i++){
			string accountType =  freeUsers[i].isAdmin == 1 ? "Admin" : "User";
			string accountStatus =  freeUsers[i].isEnabled == 1 ? "Active" : "InActive";
			cout<<i+1<<"\t "<<freeUsers[i].userName<<"\t "<<accountType<<"\t \t  "<<accountStatus<<endl;	
		}	
	}
	else{
		cout<<"No Available users"<<endl;
	}
	cout<<"............................................................................."<<endl;
	
}
void addUserToGroup(struct system *mySystem, int groupIndex){
	int currIndex = mySystem->myUsers.currentUserIndex;
	if(currIndex > 0){
		cout<<"List of available Users"<<endl;
		displayAvailableUsers(mySystemPointer, groupIndex);
		string userName;
		cout<<"Enter the Id or the username of the user"<<endl;
		cin>>userName;
		struct user newUser = getUserFromIdOrUserName(mySystemPointer, userName);
		int newIndex = ++mySystem->myGroups.groups[newIndex].groupUsers.currentUserIndex;
		mySystem->myGroups.groups[groupIndex].groupUsers.users[newIndex] = newUser;
		cout<<"Succesfully added!"<<endl;
		listGroupUser(mySystemPointer, groupIndex);	
	}
	else{
		cout<<"No Possible user to add"<<endl;
		return;
	}
	
}

int getIndexFromSystemName(struct system *mySystem, string type, string id){
	if(type== "group"){
		int currIndex = mySystem->myGroups.currentGroupIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myGroups.groups[i].groupName == id){
				return i;
			}
		}
	}
	else if(type == "users"){
		int currIndex = mySystem->myUsers.currentUserIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myUsers.users[i].userName == id){
				return i; 
			}
		}
	}
	else {
		int currIndex = mySystem->myObjects.currentObjectIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myObjects.objects[i].objectName == id){
				return i;
			}
		}
	}

}

int getIndexFromSystem(struct system *mySystem, string type, string id){
	int index = -1;
	if(type== "group"){
		int currIndex = mySystem->myGroups.currentGroupIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myGroups.groups[i].groupId == id){
				index = i;
			}
		}
	}
	else if(type == "users"){
		int currIndex = mySystem->myUsers.currentUserIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myUsers.users[i].userId == id){
				index = i;
			}
		}
	}
	else {
		int currIndex = mySystem->myObjects.currentObjectIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myObjects.objects[i].objectId == id){
				index = i;
			}
		}
	}
	return index ;
}
void enableOrDisableGroupOrUser(struct system *mySystem, string type){
	int currGroupIndex = mySystem->myGroups.currentGroupIndex;
	int currUserIndex = mySystem->myUsers.currentUserIndex;
	string enOperation;

	if(type == "group" && currGroupIndex != -1 ){
		int index;
		string operation;
		cout<<"Enter the "<<type<<" Id"<<endl;
		cin>>index;
		cout<<"Enter the operation (disable/enable)"<<endl;
		cin>>operation;
		enOperation = operation;
		mySystem->myGroups.groups[index-1].isEnabled = operation=="enable" ? 1 : 0;	
		displayAllGroups(mySystemPointer);
	}
	else if(type == "user" && currUserIndex > 0 ){
		int index;
		string operation;
		cout<<"Enter the "<<type<<" Id"<<endl;
		cin>>index;
		cout<<"Enter the operation (disable/enable)"<<endl;
		cin>>operation;
		enOperation = operation;
		mySystem->myUsers.users[index-1].isEnabled = operation=="enable" ? 1 : 0;
		displayAllUsers(mySystemPointer);	
	}
	
	if(currGroupIndex != -1 || currUserIndex > 0 ){
		string result = enOperation == "enable" ? "Enabled" : "Disabled";
		cout<< "Successfully "<<result<<endl;	
	}
	else{
		return ;
	}

}



void displayAllUsers (struct system *mySystem) {
	int currIndex = mySystem->myUsers.currentUserIndex;
	int i = 0;
	cout<<"............................................................................."<<endl;
	if(currIndex > 0 ){
		cout<<"Id \t Users \t Account Type \t Account Status"<<endl;	
		for(i=0; i< currIndex+1; i++){
			if(i == mySystem->authUserId)continue;
			string accountType =  mySystem->myUsers.users[i].isAdmin == 1 ? "Admin" : "User";
			string accountStatus =  mySystem->myUsers.users[i].isEnabled == 1 ? "Active" : "InActive";
			cout<<i+1<<"\t "<<mySystem->myUsers.users[i].userName<<"\t "<<accountType<<"\t \t  "<<accountStatus<<endl;	
		}	
	}
	else{
		cout<<"No currents users"<<endl;
	}
	cout<<"............................................................................."<<endl;
	cout<<endl;
}
void displayAllGroups(struct system *mySystem){
	int currIndex = mySystem->myGroups.currentGroupIndex;
	int i = 0;
	cout<<"............................................................................."<<endl;
	if(currIndex == -1){
		cout<<"No currents Groups"<<endl;
	}
	else{
		cout<<"Id \t Groups \t Group Status"<<endl;	
		for(i=0; i< currIndex+1; i++){
			string accountStatus =  mySystem->myGroups.groups[i].isEnabled == 1 ? "Active" : "InActive";
			cout<<i+1<<"\t "<<mySystem->myGroups.groups[i].groupName<<"\t  "<<accountStatus<<endl;	
		}
	}
	cout<<"............................................................................."<<endl;
	cout<<endl;
}



void createNewObject (struct system *mySystem, string currentUserName){
	int newIndex = ++mySystem->myObjects.currentObjectIndex;
	string fileName;
	cout<<"Input the file name that you want to create"<<endl;
	cin>>fileName;
	string filePath = "files/"+fileName; 
 	ofstream file(filePath.c_str());
	cout<<"The file was created succesfully"<<endl;
	mySystem->myObjects.objects[newIndex].objectName=fileName;
	mySystem->myObjects.objects[newIndex].objectId = gen_id();
	mySystem->myObjects.objects[newIndex].objectOwner = currentUserName;
	//initializing the defaul permissions for the file
	giveAllAdminsUserPermission(newIndex, mySystemPointer); //permission for all users
	addNewUserPermssionToObject(convertPermissionIntToString(7), currentUserName, newIndex, mySystemPointer); // permissions for the curent userName
	
}

string convertPermissionIntToString(int permit){
	string ans = "---";
	if(permit - 4 >= 0){
		ans[0]  = '4';
		permit = permit - 4;
	}
	if(permit -  2 >= 0) {
		ans[1] = '2';
		permit = permit - 2;
	}
	if(permit -1 >= 0 ){
		ans[2] = '1';
		permit = permit - 1;
	}
	return ans;
}

void giveAllAdminsUserPermission (int objectIndex, struct system *mySystem) {
	int currIndex = mySystem->myUsers.currentUserIndex;
	int i = 0;
	for(i=0; i< currIndex+1; i++){
		if(mySystem->myUsers.users[i].isAdmin){
			addNewUserPermssionToObject(convertPermissionIntToString(7), mySystem->myUsers.users[i].userName, objectIndex, mySystemPointer);
		}
	}
	
}
void displayUserDetails (struct system *mySystem, int userId){
	cout<<"............................................................................."<<endl;
	cout<<"User Name: "<< mySystem->myUsers.users[userId].userName<<endl;
	string accountType =  mySystem->myUsers.users[userId].isAdmin == 1 ? "Admin" : "User";
	string accountStatus =  mySystem->myUsers.users[userId].isEnabled == 1 ? "Active" : "InActive";
	cout<<"User Account type: "<< accountType<<endl;
	cout<<"User Status: "<< accountStatus<<endl;
	cout<<"............................................................................."<<endl;
}
struct user getCurrentUserDetails (struct system * mySystem) {
	if(mySystem->authUserId >= 0){
		return mySystem->myUsers.users[mySystem->authUserId];		
	}
	else {
		struct user fakeUser;
		fakeUser.isEnabled = 0;
		fakeUser.isAdmin = 0;
		return fakeUser;
	}		
}
struct user getUserFromIdOrUserName (struct system *mySystem, string userNameOrId){
	int index = 0;
	if(isNumber(userNameOrId)){
		stringstream newString(userNameOrId);
		newString >> index;
	}
	else index = getIndexFromSystemName(mySystemPointer, "users", userNameOrId);
	return mySystem->myUsers.users[isNumber(userNameOrId)  ? index -1 : index];
}
void listGroupUser (struct system *mySystem, int groupIndex){
	int groupUserIndex = mySystem->myGroups.groups[groupIndex].groupUsers.currentUserIndex;
	if(groupUserIndex > 0){
		cout<<"Users in the group"<<endl;
		cout<<"............................................................................."<<endl;
		cout<<"Id \t Users \t Account Type \t Account Status"<<endl;	
		for(int i=0; i< groupUserIndex+1; i++){
			if(i == mySystem->authUserId)continue;
			string accountType =  mySystem->myGroups.groups[groupIndex].groupUsers.users[i].isAdmin == 1 ? "Admin" : "User";
			string accountStatus =  mySystem->myGroups.groups[groupIndex].groupUsers.users[i].isEnabled == 1 ? "Active" : "InActive";
			cout<<i+1<<"\t "<<mySystem->myGroups.groups[groupIndex].groupUsers.users[i].userName<<"\t "<<accountType<<"\t \t  "<<accountStatus<<endl;	
		}
		cout<<"............................................................................."<<endl;		
	}
	else{
		cout<<"............................................................................."<<endl;
		cout<<"No users in the group"<<endl;
		cout<<"............................................................................."<<endl;
	}
		
}
void userLogin (struct system *mySystem){
	cout<<"User Login "<<endl;
	string userName, password;
	cout<<"Enter the user name: ";
	cin>>userName;
	cout<<"Enter the password: ";
	cin>>password;
	int currIndex = mySystem->myUsers.currentUserIndex;
	int i = 0;
	bool isLoggedIn = false;
	for(i=0; i< currIndex+1; i++){
		if(mySystem->myUsers.users[i].password == password && mySystem->myUsers.users[i].userName == userName){
			isLoggedIn = true;
			mySystem->authUserId = i;

		}
	}
	string loggedinState = isLoggedIn ? "login successful" : "login failed. Wrong username or password";
	cout<<endl;
	cout<<loggedinState<<endl;
	if(isLoggedIn){
		displayUserDetails(mySystemPointer, mySystem->authUserId);	
	}
	
	
}
void addNewUserPermssionToObject (string permission, string currentUserName, int newIndex, struct system *mySystem ){
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermission =  mySystem->myObjects.objects[newIndex].userPermission.currentUserPermission > 0  ? mySystem->myObjects.objects[newIndex].userPermission.currentUserPermission : -1; 
	int newPermissionIndex = ++mySystem->myObjects.objects[newIndex].userPermission.currentUserPermission;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canRead = permission[0]=='4' ? 1: 0;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canWrite = permission[1]=='2' ? 1: 0;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canExecute = permission[2]=='1' ? 1: 0;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].userName = currentUserName;
//	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].userId = getIdFromSystemName(mySystemPointer, "users", currentUserName);
}
void logout (struct system *mySystem){
	mySystem->authUserId = -1;
	cout<<"............................................................................."<<endl;
	cout<<"logout successfull"<<endl;
	cout<<"............................................................................."<<endl;
}

//int[] string 

//void writeObject (struct syst)

int main (){

	cout<<"Welcome to the system"<<endl;
	cout<<"Create a new Default Account"<<endl;
	addUserToSystem(mySystemPointer);
	system("CLS");
	
	int action1 = 1;
	while(action1 > 0){
		cout<<"1. Login"<<endl;
		if(mySystem.authUserId >=0){
			cout<<"2. Logout"<<endl;		
		}
		cout<<"0. Quit"<<endl;
		cin>>action1;
		if(action1 <= 0){
			exit(1);
		}
		if(action1 == 1){
			int action2 = 1;
			userLogin(mySystemPointer);
			struct user currentUser = getCurrentUserDetails(mySystemPointer);
			if(currentUser.isAdmin && mySystem.authUserId >=0){
				while(action2 > 0){
					cout<<"Admin Menu"<<endl;
					cout<<"1. Add new user/group"<<endl;
					cout<<"2. Disable/Enable a user/group"<<endl;
					cout<<"3. Add/Delete user to/grom group"<<endl;
					cout<<"4. Grant/Revoke users/groups rights"<<endl;
					cout<<"0. Return"<<endl;
					cin>>action2;
					if(action2 == 1){
						string reply;
						cout<<"Do you want to add group/user ? (user/group)"<<endl;
						cin>>reply;
						if(reply == "group"){
							addGroupToSystem(mySystemPointer);
						}
						else if(reply == "user"){
							addUserToSystem(mySystemPointer);	
						}
						else break;
						system("CLS");
					}
					if(action2 == 2){
						string reply;
						cout<<"Do you want to enable/disable (group/user)"<<endl;
						cin>>reply;
						if(reply == "group"){
							displayAllGroups(mySystemPointer);
						}
						else if(reply == "user"){
							displayAllUsers(mySystemPointer);
						}
						else break;
						enableOrDisableGroupOrUser(mySystemPointer, reply);
					}
					if(action2 == 3){
						int index;
						displayAllGroups(mySystemPointer);
						cout<<"Enter the Group Id you want to modify"<<endl;
						cin>>index;
						string operation;
						cout<<"Current users in the group"<<endl;
						listGroupUser(mySystemPointer, index);
						cout<<"Do you want to add or delete a user (add/delete) ?"<<endl;
						cin>>operation;
						if(operation == "add"){	
							addUserToGroup(mySystemPointer, index);
							
						}
						else if (operation == "delete"){
//							cout<<""
						}
						else break;
					}
					if(action2 == 0){
						break;
					}
				}
				
			}
			else {
				
			}

			//handle other functionality
		}
		if(action1 == 2 && mySystem.authUserId >= 0){
			logout(mySystemPointer);
		}

	}
	
	
	//continue tmoro
	//addGroupToSystem(mySystemPointer);
	//displayAllUsers(mySystemPointer);
	//enableOperationForUser(mySystemPointer);
	//createNewObject(mySystemPointer);
	return 0;
}

