#include <bits/stdc++.h>
#include <ctime>
#include <unistd.h>
#include <fstream>  

using namespace std;
void giveAllAdminsUserPermission(int newIndex, struct system *mySystem);
string convertPermissionIntToString(int permit);
void addNewUserPermssionToObject (string permission, string currentUserName, int newIndex, struct system *mySystem );
int authUserId = -1;
struct user {
	string userName;
	string password;
	int isAdmin;
	string userId;
	int isEnabled;
};
struct group {
	string groupId;
	string groupName;
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
struct myUsers {
	struct user users[100];
	int currentUserIndex;
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
};

struct system createNewSystem(){
	struct system mySystem;
	mySystem.myUsers.currentUserIndex = -1;
	mySystem.myGroups.currentGroupIndex = -1;
	mySystem.myObjects.currentObjectIndex = -1;
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
	cout<<"Succesfully added!"<<endl;
}

string getIdFromSystemName(struct system *mySystem, string type, string id){
	string returnId;
	if(type== "group"){
		int currIndex = mySystem->myGroups.currentGroupIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myGroups.groups[i].groupName == id){
				returnId=  mySystem->myGroups.groups[i].groupId;
			}
		}
	}
	else if(type == "users"){
		int currIndex = mySystem->myUsers.currentUserIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myUsers.users[i].userName == id){
				returnId= mySystem->myUsers.users[i].userId; 
			}
		}
	}
	else {
		int currIndex = mySystem->myObjects.currentObjectIndex;
		for(int i  = 0; i< currIndex + 1; i++){
			if(mySystem->myObjects.objects[i].objectName == id){
				returnId = mySystem->myObjects.objects[i].objectId;
			}
		}
	}
	return returnId;
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
void enableOrDisableGroupOrUser(struct system *mySystem, string type, int operation, int index){
	if(type == "group"){
		mySystem->myGroups.groups[index].isEnabled = operation;	
	}
	else{
		mySystem->myUsers.users[index].isEnabled = operation;	
	}
	string result = operation == 1 ? "Enabled" : "Disabled";
	cout<< "Successfully "<<result<<endl;
}



void displayAllUsers (struct system *mySystem) {
	int currIndex = mySystem->myUsers.currentUserIndex;
	int i = 0;
	for(i=0; i< currIndex+1; i++){
		cout<<"the user name is  "<<mySystem->myUsers.users[i].userName;
		
	}
}

void enableOperationForUser (struct system *mySystem, string type, string userName,  int operation){
	string result = type == "users" ? "User" : "Group";
	int index = getIndexFromSystem(mySystemPointer, type, getIdFromSystemName(mySystemPointer,type, userName)); 
	if(index >= 0){
		enableOrDisableGroupOrUser(mySystemPointer, type, operation, index);
	}
	else {
		cout<<result<<" is not found"<<endl;
	}
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
	if(authUserId >= 0){
		return mySystem->myUsers.users[authUserId];		
	}
	else {
		struct user fakeUser;
		fakeUser.isEnabled = 0;
		fakeUser.isAdmin = 0;
		return fakeUser;
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
			authUserId = i;

		}
	}
	string loggedinState = isLoggedIn ? "login successful" : "login failed. Wrong username or password";
	cout<<endl;
	cout<<loggedinState<<endl;
	if(isLoggedIn){
		displayUserDetails(mySystemPointer, authUserId);	
	}
	
	
}
void addNewUserPermssionToObject (string permission, string currentUserName, int newIndex, struct system *mySystem ){
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermission =  mySystem->myObjects.objects[newIndex].userPermission.currentUserPermission > 0  ? mySystem->myObjects.objects[newIndex].userPermission.currentUserPermission : -1; 
	int newPermissionIndex = ++mySystem->myObjects.objects[newIndex].userPermission.currentUserPermission;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canRead = permission[0]=='4' ? 1: 0;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canWrite = permission[1]=='2' ? 1: 0;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].canExecute = permission[2]=='1' ? 1: 0;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].userName = currentUserName;
	mySystem->myObjects.objects[newIndex].userPermission.currentUserPermissions[newPermissionIndex].userId = getIdFromSystemName(mySystemPointer, "users", currentUserName);
}
void logout (){
	authUserId = -1;
}

//int[] string 

//void writeObject (struct syst)

int main (){

	cout<<"Welcome to the system"<<endl;
	cout<<"Create a new Default Account"<<endl;
	addUserToSystem(mySystemPointer);

	
	int action1 = 1;
	while(action1 > 0){
		cout<<"1. Login"<<endl;
		cout<<"2. Logout"<<endl;
		cout<<"0. Quit"<<endl;
		cin>>action1;
		if(action1 <= 0){
			exit(1);
		}
		if(action1 == 1){
			int action2 = 1;
			userLogin(mySystemPointer);
			struct user currentUser = getCurrentUserDetails(mySystemPointer);
			if(currentUser.isAdmin && authUserId >=0){
				while(action2 > 0){
					cout<<"Admin Menu"<<endl;
					cout<<"1. Add new user/group"<<endl;
					cout<<"2. Disable/Enable a user/group"<<endl;
					cout<<"3. Add/Delete user to/grom group"<<endl;
					cout<<"4. Grant/Revoke users/groups rights"<<endl;
					cout<<"0. Return"<<endl;
					cin>>action2;
					if(action2 == 1){
						addUserToSystem(mySystemPointer);
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

	}
	
	
	
	//addGroupToSystem(mySystemPointer);
	//displayAllUsers(mySystemPointer);
	//enableOperationForUser(mySystemPointer);
	//createNewObject(mySystemPointer);
	return 0;
}

