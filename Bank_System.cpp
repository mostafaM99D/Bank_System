#include"Core.h";
using namespace std;
string ClientsFile = "Client.txt", UsersFile = "Users.txt";
enum enClientChoice { ShowAll = 1, AddNew = 2, Delete = 3, Update = 4, Find = 5, Transaction = 6, ManageUsers = 7 };
enum enUsersChoice { ShowAllUsers = 1, AddNewUsers = 2, DeleteUsers = 3, UpdateUsers = 4, FindUsers = 5 };
enum enTransactionChoice { Deposite = 1, Withdraw = 2, TotalBalances = 3, MainMenu = 4 };
enum enPermessions { all = -1, showall = 1, New = 2, delet = 4, update = 8, find = 16, transaction = 32, manageusers = 64 };
struct stClient {
	string AccountNumber = "";
	int PinCode = 0;
	string Name = "";
	long long Phone = 0;
	double AccountBalance = 0;
	bool MarkFoDeleted = 0;
};
struct stUser {
	string UserName = "";
	string Password = "";
	int Permessions = 0;
	bool MarkForDelete = 0;
};
vector<stClient> Clients;
vector<stUser> Users;
stUser CurrentUser;
void PrintClientCard(stClient Client);
void ShowMainScreen();
bool FindClientByAccountNumber(string AccountNumber, vector<stClient>clients, stClient& Client);
void ShowTransactionScreen();
void Header(string Header);
void ShowManageUsersScreen();
void PrintUserCard(stUser User);
void ShowLoginScreen();
// Read info 
stClient ReadNewClient(string accNum) {
	stClient data;
	/*cout << "Enter account number : ";
	getline(cin >> ws, data.AccountNumber);*/
	data.AccountNumber = accNum;
	cout << "Enter pin code :";
	cin >> data.PinCode;
	cout << "Enter your name : ";
	getline(cin >> ws, data.Name);
	cout << "Enter your phone : ";
	cin >> data.Phone;
	cout << "Enter your account balance :";
	cin >> data.AccountBalance;
	return data;
}
string ReadAccountNumber(string message) {
	string AccountNumber;
	cout << message;
	cin >> AccountNumber;
	return AccountNumber;
}
// Strings
vector<string> SplitString(string s, string delim)
{
	vector<string>res;
	string word = "";
	while (s.find(delim) != string::npos) {
		size_t pos = s.find(delim);
		word = s.substr(0, pos);
		if (word != "")
			res.push_back(word);
		s.erase(0, pos + delim.length());
	}
	if (s != "")
		res.push_back(s);
	return res;
}
string JoinString(string s, string delim) {
	string res = "";
	for (char& c : s) {
		if (c == ' ')
			res += delim;
		else
			res += c;
	}
	return res;
}
string JoinString(vector<string> vString, string delim) {
	string res = "";
	for (string& s : vString)
		res += s + delim;
	return res.substr(0, res.length() - delim.length());
}
//converting
string ConvertRecordToLine(stClient ClientData, string delim = "#//#") {
	string res = "";
	res += ClientData.AccountNumber + delim;
	res += to_string(ClientData.PinCode) + delim;
	res += ClientData.Name + delim;
	res += to_string(ClientData.Phone) + delim;
	res += to_string(ClientData.AccountBalance);
	return res;
}
string ConvertRecordToLine(stUser UserData, string delim = "#//#") {
	string res = "";
	res += UserData.UserName + delim;
	res += UserData.Password+delim;
	res += to_string(UserData.Permessions);
	return res;
}
stClient ConvertLineToRecord(string Line) {
	vector<string> res;
	res = SplitString(Line, "#//#");
	stClient client;
	client.AccountNumber = res[0];
	client.PinCode = stoi(res[1]);
	client.Name = res[2];
	client.Phone = stoll(res[3]);
	client.AccountBalance = stod(res[4]);
	return client;
}
stUser ConvertLineToUserRecord(string Line) {
	vector<string> res;
	res = SplitString(Line, "#//#");
	stUser user;
	user.UserName = res[0];
	user.Password = res[1];
	user.Permessions = stoi(res[2]);
	return user;
}
//file handling
void AddLineToFile(string Line, string File = ClientsFile) {
	fstream file;
	file.open(File, ios::out | ios::app);
	if (file.is_open()) {
		file << Line << endl;
		file.close();
	}
}
vector<stClient>SaveTheChangesToFile(vector<stClient>clients) {
	fstream file;
	file.open(ClientsFile, ios::out);
	if (file.is_open()) {
		string Line;
		for (stClient& c : clients) {
			if (!c.MarkFoDeleted)
			{
				Line = ConvertRecordToLine(c);
				file << Line << endl;
			}
		}
		file.close();
	}
	return clients;
}
vector<stUser>SaveTheChangesToUserFile(vector<stUser>Users, string FileName) {
	fstream file;
	file.open(FileName, ios::out);
	if (file.is_open()) {
		string Line;
		for (stUser& u : Users) {
			if (!u.MarkForDelete) {
				Line = ConvertRecordToLine(u);
				file << Line << endl;
			}
		}
		file.close();
	}
	return Users;
}
vector<stClient>LoadFromFile() {

	vector<stClient>res;
	fstream file;
	file.open(ClientsFile, ios::in);
	if (file.is_open()) {
		string line;
		stClient client;
		while (getline(file, line)) {
			if (line != "" || line != " ")
			{
				client = ConvertLineToRecord(line);
				res.push_back(client);
			}
		}
		file.close();
	}
	return res;
}
vector<stUser>LoadUsersFromFile(string FileName) {

	vector<stUser>res;
	fstream file;
	file.open(FileName, ios::in);
	if (file.is_open()) {
		string line;
		stUser User;
		while (getline(file, line)) {
			if (line != "" || line != " ")
			{
				User = ConvertLineToUserRecord(line);
				res.push_back(User);
			}
		}
		file.close();
	}
	return res;
}
void RefreshClientsInVector() {
	Clients = LoadFromFile();
}
void RefreshUsersInVector() {
	Users = LoadUsersFromFile(UsersFile);
}
// client sys
void AddNewClient() {
	stClient Client;
	string accNum = ReadAccountNumber("\nEnter account number : ");
	while (true) {
		if (FindClientByAccountNumber(accNum, Clients, Client))
		{
			cout << "Client with [" << Client.AccountNumber << "] is already exist." << endl;
			accNum = ReadAccountNumber("Enter another account number : ");
		}
		else
			break;
	}
	cout << "\n\nNow you adding a new Client :\n" << endl;
	Client = ReadNewClient(accNum);
	AddLineToFile(ConvertRecordToLine(Client));
	RefreshClientsInVector();
	cout << "\n\nClient with [" << Client.AccountNumber << "] is added sucssefully." << endl;
}
bool FindClientByAccountNumber(string AccountNumber, vector<stClient>clients, stClient& Client) {
	for (stClient& c : clients) {
		if (c.AccountNumber == AccountNumber) {
			Client = c;
			return true;
		}
	}
	return false;
}
bool MarkClientForDeleting(string AccountNumber, vector<stClient>& Clients) {
	for (stClient& c : Clients) {
		if (c.AccountNumber == AccountNumber)
		{
			c.MarkFoDeleted = true;
			return true;
		}
	}
	return false;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient> clients) {
	stClient client;
	if (FindClientByAccountNumber(AccountNumber, clients, client)) {
		PrintClientCard(client);
		char c;
		cout << "\n\n\t\t\t\t\tAre you sure you want delete this client? y/n ? ";
		cin >> c;
		if (c == 'y' || c == 'Y') {
			MarkClientForDeleting(AccountNumber, clients);
			SaveTheChangesToFile(clients);
			clients = LoadFromFile();
			cout << "\n\n\t\t\t\t\tClient Deleted Successfully.";
			RefreshClientsInVector();
			return true;
		}
		else
			cout << "\t\t\t\t\tOk, we are not delete the client" << endl;
	}
	else {
		cout << "\n\t\t\t\t\tClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
	return false;

}
stClient ChangeClientData(string AccountNumber) {
	stClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "\n\nEnter PinCode? ";
	cin >> Client.PinCode;
	cout << "Enter Name? ";
	getline(cin >> ws, Client.Name);
	cout << "Enter Phone? ";
	cin >> Client.Phone;
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}
bool UpdateClient(string AccountNumber, vector<stClient>& Clients) {
	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, Clients, Client))
	{
		PrintClientCard(Client);
		cout << "\n\n\t\t\t\t\tAre you sure you want update this client? y/n? ";
		char c;
		cin >> c;
		if (c == 'y' || c == 'Y') {

			for (stClient& cl : Clients) {
				if (cl.AccountNumber == AccountNumber) {
					cl = ChangeClientData(AccountNumber);
					break;
				}
			}
			Clients = SaveTheChangesToFile(Clients);
			cout << "\n\n\t\t\t\t\tClient Updated Successfully.";
			RefreshClientsInVector();
			return true;
		}

	}
	else {
		cout << "\n\t\t\t\t\tClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}

	return false;
}
void FindClient(string accNum) {
	stClient Client;
	if (FindClientByAccountNumber(accNum, Clients, Client)) {
		PrintClientCard(Client);
	}
	else {
		cout << "\n\t\t\t\t\tClient with Account Number (" << accNum << ") is Not Found!";
	}
}
// for users
bool FindUserByUsername(string Username, vector<stUser>Users, stUser& User) {
	for (stUser& u : Users) {
		if (u.UserName == Username) {
			User = u;
			return true;
		}
	}
	return false;
}
bool FindUserByUsernameAndPassword(string Username, string Password, vector<stUser>Users,stUser&User) {
	for (stUser& u : Users) {
		if (u.UserName == Username && u.Password == Password) {
			User = u;
			return true;
		}
	}
	return false;
}
string ReadUsername(string Message) {
	string res;
	cout << Message << endl;
	cin >> res;
	return res;
}
int ReadPermissions(stUser& User) {
	char c = ' ';
	cout << "\nDo you want to give this user all permissions ?(y/n)" << endl;
	cin >> c;
	if (c == 'y' || c == 'Y') {
		User.Permessions = enPermessions::all;
		return User.Permessions;
	}
	User.Permessions = 0;
	cout << "Do you want give this user permission to show all clients ? (y/n)" << endl;
	cin >> c;
	if (c == 'y' || c == 'Y') {
		User.Permessions += enPermessions::showall;
	}
	cout << "Do you want give this user permission to add new clients ? (y/n)" << endl;
	cin >> c;
	if (c == 'y' || c == 'Y') {
		User.Permessions += enPermessions::New;
	}
	cout << "Do you want give this user permission to show delete clients ? (y/n)" << endl;
	cin >> c;
	if (c == 'y' || c == 'Y') {
		User.Permessions += enPermessions::delet;
	}
	cout << "Do you want give this user permission to update clients? (y/n)" << endl;
	cin >> c;
	if (c == 'y' || c == 'Y') {
		User.Permessions += enPermessions::update;
	}
	cout << "Do you want give this user permission to find clients ? (y/n)" << endl;
	cin >> c;
	if (c == 'y' || c == 'Y') {
		User.Permessions += enPermessions::find;
	}
	cout << "Do you want give this user permission to transaction ? (y/n)" << endl;
	cin >> c;
	if (c == 'y' || c == 'Y') {
		User.Permessions += enPermessions::transaction;
	}
	cout << "Do you want give this user permission to manage users ? (y/n)" << endl;
	cin >> c;
	if (c == 'y' || c == 'Y') {
		User.Permessions += enPermessions::manageusers;
	}

	return User.Permessions;
}
stUser ReadNewUser(string Username) {
	stUser data;
	data.UserName = Username;
	cout << "Enter the password : ";
	cin >> data.Password;
	data.Permessions = ReadPermissions(data);
	return data;
}
bool CheckAccessPermission(enPermessions Permission)
{
	if (CurrentUser.Permessions == enPermessions::all)
		return true;
	if ((Permission & CurrentUser.Permessions) == Permission)
		return true;
	else
		return false;
}
// user sys
void AddNewUser() {
	stUser User;
	string Username = ReadUsername("\nEnter Username : ");
	while (true) {
		if (FindUserByUsername(Username, Users, User))
		{
			cout << "User with [" << User.UserName << "] is already exist." << endl;
			Username = ReadUsername("Enter another Username : ");
		}
		else
			break;
	}
	cout << "\n\nNow you adding a new User :\n" << endl;
	User = ReadNewUser(Username);
	AddLineToFile(ConvertRecordToLine(User), UsersFile);
	RefreshUsersInVector();
	cout << "\n\nUser with [" << User.UserName << "] is added sucssefully." << endl;
}
bool MarkUserForDelete(string Username, vector<stUser>& Users) {
	for (stUser& u : Users) {
		if (u.UserName == Username) {
			u.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
bool DeleteUserByUsername(string Username, vector<stUser>Users) {
	stUser User;
	if (FindUserByUsername(Username, Users, User)) {
		PrintUserCard(User);
		char c;
		cout << "\n\n\t\t\t\t\tAre you sure you want delete this User? y/n ? ";
		cin >> c;
		if (c == 'y' || c == 'Y') {
			MarkUserForDelete(Username, Users);
			SaveTheChangesToUserFile(Users, UsersFile);
			Users = LoadUsersFromFile(UsersFile);
			cout << "\n\n\t\t\t\t\tUser Deleted Successfully.";
			RefreshUsersInVector();
			return true;
		}
		else
			cout << "\t\t\t\t\tOk, we are not delete the User" << endl;
	}
	else {
		cout << "\n\t\t\t\tUser with Account Number (" << Username << ") is Not Found!";
		return false;
	}
	return false;
}
stUser ChangeUsertData(string Username) {
	stUser User;
	User.UserName = Username;
	cout << "Enter the password : ";
	cin >> User.Password;

	return User;
}
bool UpdateUser(string Username, vector<stUser>& Users) {
	stUser User;

	if (FindUserByUsername(Username, Users, User))
	{
		PrintUserCard(User);
		cout << "\n\n\t\t\t\t\tAre you sure you want update this User? y/n? ";
		char c;
		cin >> c;
		if (c == 'y' || c == 'Y') {

			for (stUser& u : Users) {
				if (u.UserName == Username) {
					u = ChangeUsertData(Username);
					break;
				}
			}
			Users = SaveTheChangesToUserFile(Users, UsersFile);
			cout << "\n\n\t\t\t\tUser Updated Successfully.";
			RefreshClientsInVector();
			return true;
		}

	}
	else {
		cout << "\n\t\t\t\tUser with Username (" << Username << ") is Not Found!";
		return false;
	}

	return false;
}
void FindUser(string Username) {
	for (stUser& u : Users) {
		if (u.UserName == Username) {
			PrintUserCard(u);
			break;
		}
	}
}
//print clients
void PrintClientCard(stClient Client) {
	cout << "\n\n\t\t\t\t\tThe following are the client details : " << endl;
	cout << "\t\t\t\t\t===========================" << endl;
	cout << "\t\t\t\t\tAccount number : " << Client.AccountNumber << endl;
	cout << "\t\t\t\t\tPin code : " << Client.PinCode << endl;
	cout << "\t\t\t\t\tName : " << Client.Name << endl;
	cout << "\t\t\t\t\tPhone number : " << Client.Phone << endl;
	cout << "\t\t\t\t\tAccount balance : " << Client.AccountBalance << endl;
	cout << "\t\t\t\t\t===========================" << endl;
}
void PrintClientRecord(stClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintAllClients(vector<stClient> clients) {
	cout << "\n\t\t\t\t\tClient List (" << clients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stClient Client : clients) {
		PrintClientRecord(Client);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}
void PrintInBalaneceMode(stClient Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintAllInBalanceMode(vector<stClient> clients) {
	cout << "\n\t\t\t\t\tClient List (" << clients.size() << ")Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stClient Client : clients) {
		PrintInBalaneceMode(Client);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

}
//print users
void PrintUserCard(stUser User) {
	cout << "\n\n\t\t\t\t\tThe following are the user details : " << endl;
	cout << "\t\t\t\t\t===========================" << endl;
	cout << "\t\t\t\t\tUsername : " << User.UserName << endl;
	cout << "\t\t\t\t\tPassword : " << User.Password << endl;
	cout << "\t\t\t\t\tPermissions : " << User.Permessions << endl;
	cout << "\t\t\t\t\t===========================" << endl;
}
void PrintUserRecord(stUser User)
{
	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(10) << left << User.Permessions;
}
void PrintAllUsers(vector<stUser> Users) {
	cout << "\n\t\t\t\tUser List (" << Users.size() << ") Users(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Username";
	cout << "| " << left << setw(10) << "Password";
	cout << "| " << left << setw(10) << "Permission";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stUser User : Users) {
		PrintUserRecord(User);
		cout << endl;
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}
void ShowAccessDeniedMessage()
{
	cout << "\n\t\t\t\t\t------------------------------------\n";
	cout << "\t\t\t\t\tAccess Denied, \n\t\t\t\t\tYou dont Have Permission To Do this, \n\t\t\t\t\tPlease Conact Your Admin.";
	cout << "\n\t\t\t\t\t------------------------------------\n";
}
//transaction screen
bool AddBalance(string AccountNumber, double Amount) {
	cout << "\n\n\t\t\t\t\tAre you sure you want update this client? y/n? ";
	char c;
	cin >> c;
	if (c == 'y' || c == 'Y') {

		for (stClient& cl : Clients) {
			if (cl.AccountNumber == AccountNumber) {
				cl.AccountBalance += Amount;
				break;
			}
		}
		Clients = SaveTheChangesToFile(Clients);
		RefreshClientsInVector();
		return true;
	}

	return false;
}
void DepositeOperation(string AccountNumber) {
	stClient Client;
	while (true)
	{
		if (!FindClientByAccountNumber(AccountNumber, Clients, Client))
		{
			std::cout << "Client With [" << AccountNumber << "] is not founded." << endl;
			AccountNumber = ReadAccountNumber("\t\t\t\t\tPlease enter account number : ");
		}
		else
			break;
	}
	PrintClientCard(Client);
	double Amount = 0;
	cout << "\t\t\t\t\tPlease enter deposite amount : ";
	cin >> Amount;
	if (AddBalance(AccountNumber, Amount)) {
		cout << "\n\n\t\t\t\t\tAmount added successfully.";
	}
	else
		cout << "\n\n\t\t\t\t\tAmount doesn't added" << endl;

}
double AmountOfTotalBalances(vector<stClient>clients) {
	double res = 0;
	for (int i = 0; i < clients.size(); ++i)
		res += clients[i].AccountBalance;
	return res;
}
void PrintTotalBalances() {
	PrintAllInBalanceMode(Clients);
	cout << "\n\n\t\t\t\t\tTotal Amounts : " << AmountOfTotalBalances(Clients) << endl << endl << endl;
}
bool PerformWithdraw(vector<stClient>& Clients, string AccountNumber, double Amount) {
	for (stClient& cl : Clients) {
		if (cl.AccountNumber == AccountNumber) {
			cl.AccountBalance -= Amount;
			break;
		}
	}
	Clients = SaveTheChangesToFile(Clients);
	return true;

}
void WithdrawOperation(string AccountNumber) {
	stClient Client;
	if (!FindClientByAccountNumber(AccountNumber, Clients, Client))
	{
		cout << "Client with account number [" << AccountNumber << "] is not founded ." << endl;
		return;
	}
	else {
		PrintClientCard(Client);
	}
	double amount = 0;
	cout << "\t\t\t\t\tEnter withdraw amount : ";
	cin >> amount;

	while (true)
	{
		if (amount > Client.AccountBalance) {
			cout << "\t\t\t\t\tYou have exceeded the balance, You can withdraw up to : " << Client.AccountBalance << endl;
			cout << "\t\t\t\t\tEnter withdraw amount : ";
			cin >> amount;
		}
		else
			break;
	}

	if (PerformWithdraw(Clients, AccountNumber, amount))
		cout << "\t\t\t\t\tWithdraw is successfull." << endl;
	else
		cout << "\t\t\t\t\tError occured in withdraw." << endl;

}
void WhatHappenAfterTransactionChoic(enTransactionChoice choice) {
	switch (choice)
	{
	case Deposite:
		system("cls");
		Header("Deposite Screen");
		DepositeOperation(ReadAccountNumber("\t\t\t\t\tPlease enter account number : "));
		system("pause>0");
		cout << "\t\t\t\t\tPress any key to return to transaction menu ..." << endl;
		ShowTransactionScreen();

		break;
	case Withdraw:

		system("cls");
		Header("Withdraw Screen");
		WithdrawOperation(ReadAccountNumber("Enter account number : "));
		system("pause>0");
		cout << "\t\t\t\t\tPress any key to return to transaction menu ..." << endl;
		ShowTransactionScreen();
		break;
	case TotalBalances:
		system("cls");
		Header("Total Balances");
		PrintTotalBalances();
		cout << "Press any key to return to transaction menu ..." << endl;
		system("pause>0");
		ShowTransactionScreen();

		break;
	default:
		system("cls");
		ShowMainScreen();
		break;
	}
}
void ShowTransactionScreen() {
	system("cls");
	Header("Transaction");
	short choice;
	cout << "\t\t\t\t\t[1] Deposite." << endl;
	cout << "\t\t\t\t\t[2] Withdraw." << endl;
	cout << "\t\t\t\t\t[3] Total balances." << endl;
	cout << "\t\t\t\t\t[4] Main menu." << endl;
	cout << "\t\t\t\t\t============================" << endl;
	do {
		cout << "\t\t\t\t\tChoose what do you want (1 to 4) : ";
		cin >> choice;
	} while (choice > 4 || choice < 1);
	WhatHappenAfterTransactionChoic((enTransactionChoice)choice);
}
//main screen
void Header(string Header) {
	cout << "\t\t\t\t\t=============================" << endl;
	cout << "\t\t\t\t\t\t " << Header << endl;
	cout << "\t\t\t\t\t=============================" << endl;
	cout << endl;
	/*cout << "============================" << endl;
	cout << "\t " << Header << endl;
	cout << "============================" << endl;
	cout << endl;*/
}
void WhatHappenAfterChoic(enClientChoice choice) {
	switch (choice)
	{
	case ShowAll:
		system("cls");
		Header("Show Clients");
		
		if (CheckAccessPermission(enPermessions::showall))
			PrintAllClients(Clients);
		else
			ShowAccessDeniedMessage();
		
		cout << "\t\t\t\t\tPress any key to return to main menu ..." << endl;
		system("pause>0");
		ShowMainScreen();
		break;
	case AddNew:
		system("cls");
		Header("Add Clients");
		if (CheckAccessPermission(enPermessions::New))
			AddNewClient();
		else
			ShowAccessDeniedMessage();
		cout << "\n\t\t\t\t\tPress any key to return to main menu ..." << endl;
		system("pause>0");
		ShowMainScreen();
		break;
	case Delete:
		system("cls");
		Header("Delete Clients");
		if (CheckAccessPermission(enPermessions::delet))
			DeleteClientByAccountNumber(ReadAccountNumber("\t\t\t\t\tEnter account number : "), Clients);
		else
			ShowAccessDeniedMessage();
		cout << "\n\t\t\t\t\tPress any key to return to main menu ..." << endl;
		system("pause>0");
		ShowMainScreen();
		break;
	case Update:
		system("cls");
		Header("Update Clients");
		if (CheckAccessPermission(enPermessions::update))
			UpdateClient(ReadAccountNumber("\t\t\t\t\tEnter account number : "), Clients);
		else
			ShowAccessDeniedMessage();
		cout << "\n\t\t\t\t\tPress any key to return to main menu ..." << endl;
		system("pause>0");
		ShowMainScreen();
		break;
	case Find:
		system("cls");
		Header("Find Clients");
		if (CheckAccessPermission(enPermessions::find))
			FindClient(ReadAccountNumber("\t\t\t\t\tEnter account number : "));
		else
			ShowAccessDeniedMessage();
		cout << "\n\t\t\t\t\tPress any key to return to main menu ..." << endl;
		system("pause>0");
		ShowMainScreen();
		break;
	case Transaction:
		system("cls");
		if (CheckAccessPermission(enPermessions::transaction))
			ShowTransactionScreen();
		else
			ShowAccessDeniedMessage();
		cout << "\n\t\t\t\t\tPress any key to return to main menu ..." << endl;
		system("pause>0");
		ShowMainScreen();
		break;
	case ManageUsers:
		system("cls");
		if (CheckAccessPermission(enPermessions::manageusers))
			ShowManageUsersScreen();
		else
			ShowAccessDeniedMessage();
		cout << "\n\t\t\t\t\tPress any key to return to main menu ..." << endl;
		system("pause>0");
		ShowMainScreen();
		break;
	default:
		system("cls");
		ShowLoginScreen();
		break;
	}
}
void ShowMainScreen() {
	system("cls");
	Header("Main Menu");
	short choice;
	cout << "\t\t\t\t\t[1] Show client list." << endl;
	cout << "\t\t\t\t\t[2] Add new client." << endl;
	cout << "\t\t\t\t\t[3] Delete Client." << endl;
	cout << "\t\t\t\t\t[4] Update client data." << endl;
	cout << "\t\t\t\t\t[5] Find client." << endl;
	cout << "\t\t\t\t\t[6] Transactions." << endl;
	cout << "\t\t\t\t\t[7] Manage users." << endl;
	cout << "\t\t\t\t\t[8] Logout." << endl;
	cout << "\t\t\t\t\t============================" << endl;
	do {
		cout << "\t\t\t\t\tChoose what do you want (1 to 8) : ";
		cin >> choice;
	} while (choice > 8 || choice < 1);
	WhatHappenAfterChoic((enClientChoice)choice);
}
// users
void WhatHappenAfterUsersChoic(enUsersChoice choice) {

	switch (choice)
	{
	case ShowAllUsers:
		system("cls");
		Header("Show Users");
		PrintAllUsers(Users);
		cout << "\t\t\t\t\tPress any key to return to manage users menu ..." << endl;
		system("pause>0");
		ShowManageUsersScreen();
		break;
	case AddNewUsers:
		system("cls");
		Header("Add Users");
		AddNewUser();
		cout << "\n\t\t\t\t\tPress any key to return to  manage users menu ..." << endl;
		system("pause>0");
		ShowManageUsersScreen();
		break;
	case DeleteUsers:
		system("cls");
		Header("Delete Users");
		DeleteUserByUsername(ReadUsername("\t\t\t\t\tEnter username : "), Users);
		cout << "\n\t\t\t\t\tPress any key to return to  manage users menu ..." << endl;
		system("pause>0");
		ShowManageUsersScreen();
		break;
	case UpdateUsers:
		system("cls");
		Header("Update Users");
		UpdateUser(ReadUsername("\t\t\t\t\tEnter username : "), Users);
		cout << "\n\t\t\t\t\tPress any key to return to  manage users menu ..." << endl;
		system("pause>0");
		ShowManageUsersScreen();
		break;
	case FindUsers:
		system("cls");
		Header("Find Users");
		FindUser(ReadUsername("\t\t\t\t\tEnter username : "));
		cout << "\n\t\t\t\t\tPress any key to return to main menu ..." << endl;
		system("pause>0");
		ShowManageUsersScreen();
		break;
	default:
		system("cls");
		ShowMainScreen();
		break;
	}
}
void ShowManageUsersScreen() {
	system("cls");
	Header("Manage Users");
	short choice;
	cout << "\t\t\t\t\t[1] List users." << endl;
	cout << "\t\t\t\t\t[2] Add new user." << endl;
	cout << "\t\t\t\t\t[3] Delete user." << endl;
	cout << "\t\t\t\t\t[4] Update user." << endl;
	cout << "\t\t\t\t\t[5] Find user." << endl;
	cout << "\t\t\t\t\t[6] Main menu." << endl;
	cout << "\t\t\t\t\t============================" << endl;
	do {
		cout << "\t\t\t\t\tChoose what do you want (1 to 6) : ";
		cin >> choice;
	} while (choice > 6 || choice < 1);
	WhatHappenAfterUsersChoic((enUsersChoice)choice);
}
void Login(string Username, string Password) {

	while (true) {
		if (!FindUserByUsernameAndPassword(Username, Password, Users,CurrentUser)) {
			system("cls");
			Header("Login Screen");
			cout << "Invalid username / password .\n\n";
			Username = ReadUsername("\t\t\t\t\tEnter username : ");
			Password = ReadUsername("\t\t\t\t\tEnter password : ");
		}
		else
			break;
	}
	ShowMainScreen();
}
void ShowLoginScreen() {
	Header("Login Screen");
	string Username, Password;
	cout << "\t\t\t\t\tEnter username : \n";
	cin >> Username;
	cout << "\t\t\t\t\tEnter password : \n";
	cin >> Password;
	Login(Username, Password);
}
//main
int main() {
	Clients = LoadFromFile();
	Users = LoadUsersFromFile(UsersFile);
	ShowLoginScreen();
}