#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

class Account{
    public:
        long acc_no;
        string names;
        string dob;
        string issue_date;
        float balance;

        long generate_acc_no(){
            return rand()% 10000000000 + 100000000;
        }

        string generate_issue_date(){
            auto t = time(nullptr);
            auto tm = *localtime(&t);
            stringstream currentTime;
            currentTime<<put_time(&tm, "%d-%m-%Y");
            return currentTime.str();
        }

        string toString(){
            return to_string(acc_no) + "," + names + "," + dob + "," + issue_date + "," + to_string(balance);
        }

        Account(){
            acc_no = generate_acc_no();
            issue_date = generate_issue_date();
        }
};

void save_account_to_file(vector<Account> accounts){
    fstream file;
    file.open("accounts.csv", ios::in);
    
    if(!file){
        file.close();
        file.open("accounts.csv", ios::out);
        file<<"Account Number,Date of Birth,Issue Date,Balance"<<endl;
    } else {
        file.close();
        file.open("accounts.csv", ios::app);
    }

    for(auto account : accounts){
        file<<account.toString()<<endl;
    }

    file.close();
}

void create_account(vector<Account> accounts){
    int number_of_accounts;
    cout<<"Enter the number of accounts: ";
    cin>>number_of_accounts;

    for(auto i=0; i<number_of_accounts; i++){
        Account *acc = new Account();

        cout<<"Enter the account holder names: ";
        cin.ignore();
        getline(cin, acc->names);

        cout<<"Enter the date of birth: ";
        cin>>acc->dob;

        cout<<"Enter the initial balance: ";
        cin>>acc->balance;

        cout<<"Account number for "<<acc->names<<" is: "<<acc->acc_no<<" issued on "<<acc->issue_date<<endl;

        accounts.push_back(*acc);
        
        fstream logs;
        logs.open("logs.txt", ios::app);
        logs<<acc->toString()<<endl;   
        logs.close();
    }
    save_account_to_file(accounts);
}

string find_account(long account_number){
    fstream file;
    file.open("accounts.csv", ios::in);
    string line;
    string result = "";
    //find account in file
    while(getline(file, line)){
        if(line.find(to_string(account_number)) != string::npos){
            result = line;
        }
    }
    file.close();
    
    return result;
}

void check_balance() {
    long account_number;
    cout<<"Enter the account number: ";
    cin>>account_number;

    string account = find_account(account_number);
    if(account == ""){
        cout<<"Account not found"<<endl;
    } else {
        stringstream ss(account);
        string token;
        int i = 0;
        while(getline(ss, token, ',')){
            if(i == 3){
                cout<<"Balance: "<<token<<endl;
            }
            i++;
        }
    }
}


vector<Account> load_account_from_file(){
	vector<Account> accounts;
    fstream file;
    file.open("accounts.csv", ios::in);
    string line = "";

        while(getline(file, line)){
            if(line.find("Account Number") != string::npos){
                continue;
            }

            stringstream ss(line);
            string token;
            vector<string> tokens;
            while(getline(ss, token, ',')){
                if(token == ""){
                    goto close_shit;
                }
                tokens.push_back(token);
            }
            Account account;
            account.acc_no = stol(tokens[0]);
            account.dob = tokens[1];
            account.issue_date = tokens[2];
            account.balance = stof(tokens[3]);
            accounts.push_back(account);
    
            
        }
        close_shit:
            file.close();
    return accounts;
}

void account_list() {
    vector<Account> accounts = load_account_from_file();

    if (accounts.empty()){
        cout<<"No accounts found"<<endl;
    } else {
        cout<<"Account Number,Date of Birth,Issue Date,Balance"<<endl;
        for(auto account : accounts){
            cout<<account.toString()<<endl;
        }
    }
}

void account_list_alphabetically(){
    vector<Account> accounts = load_account_from_file();

    if (accounts.empty()){
        cout<<"No accounts found"<<endl;
    } else {
        //sort vector alphabetically from name
        sort(accounts.begin(), accounts.end(), [](Account a, Account b){
            return a.names < b.names;
        });

        cout<<"Account Number,Date of Birth,Issue Date,Balance"<<endl;
        for(auto account : accounts){
            cout<<account.toString()<<endl;
        }

    }
}

void account_logs(){
    long account_number;
    cout<<"Enter the account number: ";
    cin>>account_number;

    bool hasRecord = false;
    string account = find_account(account_number);
    if(account == ""){
        cout<<"Account not found"<<endl;
    } else {
        fstream file;
        file.open("logs.txt", ios::in);

        while (getline(file, account)) {
            if(account.find(to_string(account_number)) != string::npos){
                hasRecord = true;
                cout<<account<<endl;
            }
        }
        file.close();
        if(!hasRecord){
            cout<<"No records found"<<endl;
        }
    }
}

void update_balance(string action){
    long account_number;
    cout<<"Enter the account number: ";
    cin>>account_number;

    string account = find_account(account_number);
    if(account == ""){
        cout<<"Account not found"<<endl;
    } else {
        vector<Account> accounts = load_account_from_file();
        if (accounts.empty()){
            cout<<"No accounts found"<<endl;
        } else {
            for(auto account : accounts){
                if(account.acc_no == account_number){
                    if(action == "deposit"){
                        float amount;
                        cout<<"Enter the amount to deposit: ";
                        cin>>amount;
                        account.balance += amount;
                    } else if(action == "withdraw"){
                        float amount;
                        cout<<"Enter the amount to withdraw: ";
                        cin>>amount;
                        account.balance -= amount;
                    }
                }
                fstream logs;
                logs.open("logs.txt", ios::app);
                //save account to logs
                logs<<account.toString()<<endl;
                logs.close();
                
            }
            remove("accounts.csv");
            save_account_to_file(accounts);
        }
    }
}

void modify_account(){
    long account_number;
    cout<<"Enter the account number: ";
    cin>>account_number;

    string account = find_account(account_number);
    if(account == ""){
        cout<<"Account not found"<<endl;
    } else {
        vector<Account> accounts = load_account_from_file();
        if (accounts.empty()){
            cout<<"No accounts found"<<endl;
        } else {
            for(auto account : accounts){
                if(account.acc_no == account_number){
                    cout<<"Enter the new account holder names: ";
                    cin.ignore();
                    getline(cin, account.names);

                    cout<<"Enter the new date of birth: ";
                    cin>>account.dob;

                    fstream logs;
                    logs.open("logs.txt", ios::app);
                    logs<<account.toString()<<endl; 
                    logs.close(); 
                }
            }
            remove("accounts.csv");
            save_account_to_file(accounts);
        }
    }
}

void delete_account(){
    long account_number;
    cout<<"Enter the account number: ";
    cin>>account_number;

    string account = find_account(account_number);
    if(account == ""){
        cout<<"Account not found"<<endl;
    } else {
        vector<Account> accounts = load_account_from_file();
        if (accounts.empty()){
            cout<<"No accounts found"<<endl;
        } else {
            for (auto i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].acc_no == account_number)
                {
                    accounts.erase(accounts.begin() + i);
                    fstream logs;
                    logs.open("logs.txt", ios::app);
                    logs<<account<<endl;  
                    logs.close();
                }
            }
            
            remove("accounts.csv");
            save_account_to_file(accounts);
        }
    }
}

int main() {
    int option;
    vector<Account> accounts;

    do{
        cout <<"\t===== Welcome to the bank management system =====" << endl;
        cout<<"\t|\t\t1. Create an account\t\t|" << endl;
        cout<<"\t|\t\t2. Deposit\t\t\t|" << endl;
        cout<<"\t|\t\t3. Withdraw\t\t\t|" << endl;
        cout<<"\t|\t\t4. Check balance\t\t|" << endl;
        cout<<"\t|\t\t5. Modify account\t\t|" << endl;
        cout<<"\t|\t\t6. close account\t\t|" << endl;
        cout<<"\t|\t\t7. Account list\t\t\t|" << endl;
        cout<<"\t|\t\t8. Account list alphabetically\t|" << endl;
        cout<<"\t|\t\t9. Account logs\t\t\t|" << endl;
        cout<<"\t|\t\t10. Exit\t\t\t|" << endl;

        cout<<"\tEnter your option: ";
        cin>>option;

        system("cls");

        switch (option)
        {
        case 1:
            create_account(accounts);
            break;
        case 2:
            update_balance("deposit");
            break;
        case 3:
            update_balance("withdraw");
            break;
        case 4:
            check_balance();
            break;
        case 5:
            modify_account();
            break;
        case 6:
            delete_account();
            break;
        case 7:
            account_list();
            break;
        case 8:
            account_list_alphabetically();
            break;
        case 9:
            account_logs();
            break;
        case 10:
            cout<<"Thank you for using the bank management system"<<endl;
            exit(0);
            break;
        default:
            break;
        }
    }while(option!=10);

    return 0;
}