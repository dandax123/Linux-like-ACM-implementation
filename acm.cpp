#include <bits/stdc++.h>
#include <ctime>
#include <unistd.h>
using namespace std;
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
	int canRead;
	int canWrite;
	int canExecute;
};


struct groupPermission {
	string groupId;
	int canRead;
	int canWrite;
	int canExecute;
};


struct object {
	string objectId;
	string objectOwner;
	string objectName;
	struct userPermission currentUserPermissions[20];
	struct groupPermission currentGroupPermissions[20];
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
	cout<<"Enter the user Name \n";
	cin>>mySystem->myUsers.users[newIndex].userName;
	cout<<"Enter  your password \n";
	cin>>mySystem->myUsers.users[newIndex].password;
	cout<<"Are you an admin ?(Y/n) \n";
	cin>>isAdmin;
	mySystem->myUsers.users[newIndex].isAdmin = isAdmin == "Y" ? 1 : 0;
	mySystem->myUsers.users[newIndex].isEnabled = 1;
	mySystem->myUsers.users[newIndex].userId = gen_id();
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
	int index = 0;
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
	return index;
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

void enableOperationForUser (struct system *mySystem){
	string userName;
	cout<<"Enter the user Name of the user"<<endl;
	cin>>userName;
	int index = getIndexFromSystem(mySystemPointer, "users", getIdFromSystemName(mySystemPointer,"users", userName)); 
	cout<<index;
}


int main (){

	addUserToSystem(mySystemPointer);
	addGroupToSystem(mySystemPointer);
	displayAllUsers(mySystemPointer);
	enableOperationForUser(mySystemPointer);
	return 0;
}

