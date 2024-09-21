
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";

const string UsersFileName = "User.txt";

void ShowMainMenue(short p);
void ShowTransactionMenu(short p);
void Login();
void ShowManageMainMenu(int p);

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}





void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}




bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}


bool DepositAmount(vector <sClient> VClient,string AccountNumber, double AddMoney) {
    
    char Answer;
    cout << "Are you Sure Perform this Transaction \n ";
    cin >> Answer;
    if (Answer=='y')
    {
        for (sClient& C : VClient) {
            if (C.AccountNumber==AccountNumber)
            {
                C.AccountBalance += AddMoney;
                SaveCleintsDataToFile(ClientsFileName, VClient);
                cout << "Done Seccesfull New Balance is = " << C.AccountBalance << endl;
                return true;
            }
        }
    }
    return false;
   
}

void ShowDeposit() {
    cout << "_____________________________________________\n";
    cout << "\t\t Deposit Secreen\n";
    cout << "_____________________________________________\n";
    vector <sClient> VClient = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber;
    sClient Client;
    AccountNumber = ReadClientAccountNumber();
    while (!(FindClientByAccountNumber(AccountNumber,VClient,Client)))
    {
        cout << "\nSorrey This AccountNumber Does Not Exist = " << AccountNumber << endl;
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);
    double AddMoney;
    cout << "Please Enter Deposit Money \n";
    cin >> AddMoney;
    DepositAmount(VClient, AccountNumber, AddMoney);
}


void ShowWithDrawAmount() {
    cout << "_________________________________________\n";
    cout << "\t\t WithDraw Amount \n";
    cout << "____________________________________________\n";
    vector <sClient> VClient = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;
    while (!(FindClientByAccountNumber(AccountNumber,VClient,Client)))
    {
        cout << "This Client [" << AccountNumber << "]Does Not Exiest \n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);
    double AddMoney;
    cout << "Please Enter Money WithDraw\n";
    cin >> AddMoney;

    while (Client.AccountBalance<AddMoney)
    {
        cout << "Sorrey This Amount Up To your Account Balance = " << Client.AccountNumber << endl;
        cout << "please Enter Another Amount \n";
        cin >> AddMoney;
    }
    DepositAmount(VClient, AccountNumber, AddMoney * -1);
}

void PrintRecordBalanceList(vector <sClient> Vclient) {

    for (sClient& C : Vclient) {
        cout << "|" << setw(15) << left << C.AccountNumber;
        cout << "|" << setw(40) << left << C.Name;
        cout << "|" << setw(12) << left << C.AccountBalance << endl;
    }
}

double CalclateTotalBalance(vector <sClient> Vclient) {
    double S = 0;
    for (sClient& C : Vclient) {
        S += C.AccountBalance;
    }
    return S;
}

void ShowClientBalanceList() {
    vector <sClient> Vclient = LoadCleintsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t Balance Client List (" << Vclient.size() << ") Client (s)\n";
    cout << "\n_______________________________________________________________________________________\n\n";
    cout << "|" << setw(15) << left << "AccountNumber " << "|" << setw(40) << left << "Name" << "|" << setw(12) << left << " Balance";
    cout << "\n_______________________________________________________________________________________\n\n";
    PrintRecordBalanceList(Vclient);
    cout << "_________________________________________________________________________________________\n\n";
    cout << "\t\t\t\t Total Balances = " << CalclateTotalBalance(Vclient) << endl;
}


void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactionMenu = 6,
    eManageUsers = 7, Logout = 8
};

void GoBackToMainMenue(short p)
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue(p);

}

enum enTransactionOption
{
    deposit = 1, withdraw = 2 , listBalance = 3, BackMainMenu = 4
};
short ReadTransactionMenu() {
    cout << "Choose what do you want to do? [1 to 4] ?\n";
    short N =0;
    cin >> N;
    return N;
}

void GoBackToTrancactionMenu(short p) {
    cout << "Preass Any Key To Go Back To Transaction Menu\n";
    system("pause > 0");
    ShowTransactionMenu(p);
}

void PerFormTransactionMenu(enTransactionOption N,short p) {
    switch (N)
    {
    case deposit:
        system("cls");
        ShowDeposit();
        GoBackToTrancactionMenu(p);
        break;
    case withdraw:
        system("cls");
        ShowWithDrawAmount();
        GoBackToTrancactionMenu(p);
        break;
    case listBalance:
        system("cls");
        ShowClientBalanceList();
        GoBackToTrancactionMenu(p);
        break;
    case BackMainMenu:
        system("cls");
        ShowMainMenue(p);
        break;
    default:
        break;
    }
}

void ShowTransactionMenu(short p) {
    system("cls");
    cout << "======================================================\n";
    cout << "\n\t\t\t Transaction Menue Secreen \n";
    cout << "\n=====================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] WithDraw.\n";
    cout << "\t[3] Total Balance\n";
    cout << "\t[4] Main Menu\n";
    cout << "\n=======================================================\n";
    PerFormTransactionMenu((enTransactionOption)ReadTransactionMenu(),p);
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

/////////////////////////////////////////////////////////////////////////

// Manage Users .......................

enum enpermitions
{
    Show = 1, Add = 2, Delete = 4, Update = 8, Find = 16, Transactions = 32 , MangeUser =64
};

string ReadString(string massage) {
    string name;
    cout << massage << " : ";
    cin >> name;
    return name;
}

short permitionN() {
    short p = 0;
    char y ;
    cout << "Do You Want Give Full permition ? y/n  ";
    cin >> y;
    if (y == 'y')
    {
      
        return -1 ;
    }
    else
    {
        cout << "Do you Want permtion To (show) Client y/n  ";
        cin >> y;
        if (y == 'y')
        {
            p +=1;
        }
        cout << "Do you Want permtion To (Add) Client y/n  ";
        cin >> y;
        if (y == 'y')
        {
            p += 2;
        }

        cout << "Do you Want permtion To (Delete) Client y/n  ";
        cin >> y;
        if (y == 'y')
        {
            p += 4;
        }
        cout << "Do you Want permtion To (Uptade) Client y/n  ";
        cin >> y;
        if (y == 'y')
        {
            p += 8;
        }

        cout << "Do you Want permtion To (Find) Client y/n  ";
        cin >> y;
        if (y == 'y')
        {
            p += 16;
        }
        cout << "Do you Want permtion To (Transaction) Client y/n  ";
        cin >> y;
        if (y == 'y')
        {
            p += 32;
        }
        cout << "Do you Want permtion To (Manage User) Client y/n  ";
        cin >> y;
        if (y == 'y')
        {
            p += 64;
        }
    }
    return p;
}

struct stUsers
{
    string username;
    string password;
    short permtionN;
    bool MarkUser = false;
};

////////////////////////////////////////////////
// Add Users

string ConvertUserDataToLineString(stUsers User, string delmit) {
    string line;
    line += User.username + delmit;
    line += User.password + delmit;
    line +=to_string (User.permtionN);
    return line;
}


void SaveUserToFile(stUsers User) {
    string line = ConvertUserDataToLineString(User, "#//#");
    fstream MyFile;
    MyFile.open(UsersFileName, ios::out | ios::app );
    if (MyFile.is_open())
    {
        MyFile << line << endl;
    }
    MyFile.close();
}

stUsers ConvertLineStringToRecordUser(string line) {
    stUsers User;
    vector <string> Str;
    Str = SplitString(line, "#//#");
    User.username = Str[0];
    User.password = Str[1];
    User.permtionN = stoi(Str[2]);
    return User;
}

vector <stUsers> LoadDataFromFileUsers() {
    vector <stUsers> User;
    string line;
    fstream MyFile;
    stUsers C;
    MyFile.open(UsersFileName, ios::in);
    if (MyFile.is_open())
    {
        while (getline(MyFile, line))
        {

            C = ConvertLineStringToRecordUser(line);
            User.push_back(C);
        }
    }

    MyFile.close();
    return User;
}

bool IsUserExist( stUsers& Us) {
    vector <stUsers> User;

    User = LoadDataFromFileUsers();
    Us.username = ReadString("Enter UserName ");
    Us.password = ReadString("Enter Password");
    for (stUsers& C : User) {
        if (C.username == Us.username && C.password == Us.password)
        {
            Us = C;
            return true;
        }
        
    }
    return false;
}

void PrintUserCard(stUsers User) {
    cout << "\n This User Info\n\n";
    cout << "____________________________________________________\n";
    cout << "UserName   : " << User.username << endl;
    cout << "Passwor    : " << User.password << endl;
    cout << "Permission : " << User.permtionN << endl;
    cout << "\n\n_________________________________________________\n";
}
///////////////////////
// [1]Show List User

void ShowListUsers() {
    vector <stUsers> User;
    User = LoadDataFromFileUsers();
    cout << "\n\t\t\t Users List (" << User.size() << ") User (s).\n\n";
    cout << "_____________________________________________________________________\n\n";
    cout << "|" << left<<setw(15) << "User Name" << "|" << setw(10) << "Password" << "|" << setw(10) << "permitions" << endl;
    cout << "_____________________________________________________________________\n\n";
    for (stUsers& C : User)
        cout << "|" << left<<setw(15) << C.username << "|" << setw(10) << C.password << "|" <<setw(10) <<C.permtionN << endl;
}

// End Show Users

/////////////////////
// [2] Add User
void AddNewUser() {
    char y = 'y';
    while (y=='y')
    {
       
        stUsers Us;
        if (!IsUserExist(Us))
        {
            Us.permtionN = permitionN();
            SaveUserToFile(Us);
            cout << "Do you Want Add More Users ? :";
            cin >> y;
        }
        else
        {
            cout << "\n Sorry User is already Found :-( \n";
            PrintUserCard(Us);
            cout << "\nDo You Want Add Another User ? y/n \n";
            cin >> y;
            system("cls");
        }
       
    }
}

void ShowAddUser() {
    cout << "\n________________________________________________________\n";
    cout << "\n\t\t Add New User Secreen\n";
    cout << "\n________________________________________________________\n";
    cout << "\n Add New User\n\n";
    AddNewUser();
}

// End Add User

//////////////////////////////////
//[3] Delete User

bool MarkUser(vector <stUsers> &User,stUsers Us) {
    for(stUsers &C:User)
        if (C.username == Us.username && C.password == Us.password)
        {
            C.MarkUser = true;
            return true;
        }
    return false;
}

void  SaveUsersVecorToFile(vector <stUsers> &User) {
    fstream MyFile;
    string line;
    MyFile.open(UsersFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stUsers& C : User)
            if (C.MarkUser == false)
            {
                line = ConvertUserDataToLineString(C, "#//#");
                MyFile << line << endl;
            }
    }
    MyFile.close();
}

bool DeleteUser() {
    char y;
    stUsers User;
    vector <stUsers> VUser;
    
    if (IsUserExist(User))
    {
       
        if (User.username == "Amjad")
        {
            cout << "Sorry You canot Delete this User \n";
            return false;
        }
        PrintUserCard(User);
        cout << "Are you Sure Delete This User y/n ?\n";
        cin >> y;
        if (y=='y')
        {
            VUser = LoadDataFromFileUsers();
            MarkUser(VUser, User);
            SaveUsersVecorToFile(VUser);
            cout << "Deleted Is Seccessfuly\n";
        }
       
        return true;
    }
    else {
        cout << "Sorrey Not Found this User  \n";
       
    }
    return false;
}

void ShowDeleteUser() {
    cout << "\n_______________________________________________\n";
    cout << "\n\t\t Delete User\n";
    cout << "\n_______________________________________________\n";
    DeleteUser();
}

// End Delete User


// [4] Update User

stUsers UpdateData(stUsers User) {
    User.password = ReadString("Enter Passwor");
    User.permtionN = permitionN();
    return User;
}


bool UpdateUser() {
    vector <stUsers> User;
    stUsers Us;
    char  y;
    User = LoadDataFromFileUsers();
    if (IsUserExist(Us))
    {
        PrintUserCard(Us);
       
        cout << "Are you sure Update this user \n";
        cin >> y;
         if (y == 'y')
           {
             for (stUsers& C : User)
                 if (Us.username == C.username && Us.password == C.password) {
                     C = UpdateData(C);
                     break;
                 }
            }
              
           SaveUsersVecorToFile(User);
        cout << "\nUpDate Seccesfull\n";
        return true;
    }
   
    else {
        cout << "Sorrey Not Found User press any key to back \n";
        system("pause>0");
    }
    return false;

}


void ShowUpdateUser() {
    cout << "\n_______________________________________________\n";
    cout << "\n\t\t Updat User\n";
    cout << "\n_______________________________________________\n";
    UpdateUser();
}
// End Update User.....

// Find User

bool FindUser(stUsers Us) {
    if (IsUserExist(Us))
    {
        PrintUserCard(Us);
        return true;
    }
    else
    {
        cout << "Sorry This User Not Found press any key to back menu\n";
        system("pause>0");
        return false;
    }
}

void ShowFindUser() {
    cout << "\n_______________________________________________\n";
    cout << "\n\t\t Find User\n";
    cout << "\n_______________________________________________\n";
    stUsers User;
    FindUser(User);
}

// End Find User

enum enMangeUser
{
    enlistuser = 1, enAdduser = 2 , enDeleteUser = 3,enUpdateUser =4,enFindUser =5 ,enBackMainMenu = 6
};

short ReadManageUserMenu() {
    cout << "Choose what do you want to do? [1 to 6] ?\n";
    short N = 0;
    cin >> N;
    return N;
}

void GoBackToMangeMenu(int p) {
    cout << "press any key to back menu\n";
    system("pause>0");
    ShowManageMainMenu(p);
}

void performeMangeUserOption(enMangeUser M,int p) {
    switch (M)
    {
    case enlistuser:
        system("cls");
        ShowListUsers();
        GoBackToMangeMenu(p);
        break;
    case enAdduser:
        system("cls");
        ShowAddUser();
        GoBackToMangeMenu(p);
        break;
    case enDeleteUser:
        system("cls");
        ShowDeleteUser();
        GoBackToMangeMenu(p);
        break;
    case enUpdateUser:
        system("cls");
        ShowUpdateUser();
        GoBackToMangeMenu(p);
        break;
    case enFindUser:
        system("cls");
        ShowFindUser();
        GoBackToMangeMenu(p);
        break;
    case enBackMainMenu:
        system("cls");
        ShowMainMenue(p);
        break;
    default:
        break;
    }
}

void ShowManageMainMenu(int p) {
    system("cls");
    cout << "\n________________________________________________\n";
    cout << "\t\t Manage Main Menu\n";
    cout << "\n________________________________________________\n\n";
    cout << "\t[1] List Users\n";
    cout << "\t[2] Add New User \n";
    cout << "\t[3] Delete User \n";
    cout << "\t[4] Update User \n";
    cout << "\t[5] Find User \n";
    cout << "\t[6] Main Menue \n";
    cout << "\n________________________________________________\n\n";
    performeMangeUserOption(enMangeUser(ReadManageUserMenu()),p);
}

////////////////////////////////////////////////////


void PerfromMainMenueOption(enMainMenueOptions MainMenueOption, short p)
{
    
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
        if ((p & enpermitions::Show) == enpermitions::Show )
        {
            system("cls");
            ShowAllClientsScreen();
            GoBackToMainMenue(p);
            break;
        }
        else
        {
            cout << "Soreey denied \n";
            GoBackToMainMenue(p);
            break;
        }
       

    case enMainMenueOptions::eAddNewClient:
        if ((p & enpermitions::Add) == enpermitions::Add)
        {
            system("cls");
            ShowAddNewClientsScreen();
            GoBackToMainMenue(p);
            break;
        }
        else
        {
            cout << "Sorrey denied \n";
            GoBackToMainMenue(p);
            break;
        }

    case enMainMenueOptions::eDeleteClient:
        if ((p & enpermitions::Delete) == enpermitions::Delete)
        {
            system("cls");
            ShowDeleteClientScreen();
            GoBackToMainMenue(p);
            break;
        }
        else
        {
            cout << "Soreey denied \n";
            GoBackToMainMenue(p);
            break;
        }
    case enMainMenueOptions::eUpdateClient:
        if ((p & enpermitions::Update) == enpermitions::Update)
        {
            system("cls");
            ShowUpdateClientScreen();
            GoBackToMainMenue(p);
            break;
        }
        else
        {
            cout << "Soreey denied \n";
            GoBackToMainMenue(p);
            break;
        }
    case enMainMenueOptions::eFindClient:
        if ((p & enpermitions::Find) == enpermitions::Find)
        {
            system("cls");
            ShowFindClientScreen();
            GoBackToMainMenue(p);
            break;
        }
        else
        {
            cout << "Soreey denied \n";
            GoBackToMainMenue(p);
            break;
        }
    case enMainMenueOptions::eTransactionMenu:
        if ((p & enpermitions::Transactions) == enpermitions::Transactions)
        {

            system("cls");
            ShowTransactionMenu(p);
            GoBackToMainMenue(p);
            break;
        }
        else
        {
            cout << "Soreey denied \n";
            GoBackToMainMenue(p);
            break;
        }
    case enMainMenueOptions::eManageUsers:
        if ((p & enpermitions::MangeUser) == enpermitions::MangeUser)
        {
            system("cls");
            ShowManageMainMenu(p);
            GoBackToMainMenue(p);
            break;
        }
        else
        {
            cout << "Soreey denied \n";
            GoBackToMainMenue(p);
            break;
        }

    case enMainMenueOptions::Logout:
        system("cls");
        Login();
        break;
    }
}

///////////////////////////////////////////////////////////////

void ShowMainMenue(short p)
{
    
        system("cls");
        cout << "===========================================\n";
        cout << "\t\tMain Menue Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] Show Client List.\n";
        cout << "\t[2] Add New Client.\n";
        cout << "\t[3] Delete Client.\n";
        cout << "\t[4] Update Client Info.\n";
        cout << "\t[5] Find Client.\n";
        cout << "\t[6] Transaction\n";
        cout << "\t[7] Manage Users. \n";
        cout << "\t[8] Logout.\n";
        cout << "===========================================\n";
        PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption(), p);
}
void  Login() {

    cout << "\n____________________________________________________\n";
    cout << "\t\t Login Secreen\n ";
    cout << "\n____________________________________________________\n";
    stUsers User;
    if (!IsUserExist( User)) {
        
        cout << "\nSorrey Invald User \n"; 
        Login();
    }
    else
    {
        
        ShowMainMenue(User.permtionN);
    }
}
int main()

{
    Login();
 
   system("pause>0");
    return 0;
}