// refer to file `plan.txt` for the coding
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <iomanip>
using namespace std;

// declarations
class Account;
class Transaction;
class FileOperations;

string get_current_date();
string get_current_time();
Account getAccount(long int);
Transaction getTransaction(long int);

int getAccountsLen();
int getTransactionsLen();
int getAccountLineNo(long int);
int getTransactionLineNo(long int);
long randomDigits();

void loadAllAccounts(bool load = false);
void loadAllTransactions(bool load = false);

// global vars
const string ACCOUNT_FILE = "accs.txt";
const string TXNS_FILE = "txns.txt";
map<long int, Account> accounts;         // key is account number
map<long int, Transaction> transactions; // key is transaction number
const int encryptKey = 5;

class FileOperations
{
    public:
        string readFile(string filename);
        string getLineContent(string, string, int);
        void writeFileAppend(string, string);
        void writeFileOverwriteLine(string, string, int);
        string encrypt(string);
        string decrypt(string);
};

FileOperations fileOps;
// class definitions
class Account
{

protected:
    long int acc_no;
    string acc_type, acc_status;
    float balance, interest_rate;
    typedef struct
    {
        string name, address, phone, email;
    } customer;
    customer holder;

public:
    Account()
    {
        acc_no = 0;
        acc_type = "";
        acc_status = "";
        balance = 0;
        interest_rate = 0;
        holder.name = "";
        holder.address = "";
        holder.phone = "";
        holder.email = "";
        // load_transactions();
    }

    void viewTransactions(int); // view n number of transactions of an account


    void register_acc()
    {
        // generate account number
        acc_no = randomDigits();
        std::cout << "Account number: " << acc_no << endl;
        std::cout << "Enter account type[1 for savings, 2 for current]: ";
        int type;
        std::cin >> type;
        if (type == 1)
        {
            acc_type = "savings";
            interest_rate = 4.0;
        }
        else if (type == 2)
        {
            acc_type = "current";
            interest_rate = 0.0;
        }
        else
        {
            std::cout << "Invalid account type" << endl;
            return;
        }

        std::cout << "Enter account holder name: ";
        std::cin.ignore();
        getline(std::cin, holder.name);
        std::cout << "Enter account holder address: ";
        getline(std::cin, holder.address);
        std::cout << "Enter account holder phone number: ";
        getline(std::cin, holder.phone);
        std::cout << "Enter account holder email: ";
        getline(std::cin, holder.email);
        std::cout << "Enter initial deposit amount: ";
        std::cin >> balance;
        acc_status = "active";

        // write to file
        string contents = to_string(acc_no) + "," + acc_type + "," + acc_status + "," + to_string(balance) + "," + to_string(interest_rate) + "," + holder.name + "," + holder.address + "," + holder.phone + "," + holder.email;
        fileOps.writeFileAppend(ACCOUNT_FILE, contents);
    }

    long int get_acc_no()
    {
        if (acc_status == "inactive")
        {
            std::cout << "Account is inactive" << endl;
            return -1;
        }
        return acc_no;
    }

    float get_balance()
    {
        return balance;
    }

    void display_account()
    {
        system("clear");
        // use iomanip to format the output in a nice tabular form with proper headings
        if (holder.address.length() > 35)
        {
            holder.address = holder.address.substr(0, 30) + "...";
        }

        if (holder.email.length() > 20)
        {
            holder.email = holder.email.substr(0, 15) + "...";
        }
        int address_len = holder.address.length();
        std::cout << left << setw(15) << "Account No." << setw(15) << "Account Type" << setw(15) << "Account Status" << setw(15) << "Balance" << setw(15) << "Interest Rate" << setw(20) << "Name" << setw(address_len + 1) << "Address" << setw(15) << "Phone" << setw(20) << "Email" << endl;
        std::cout << left << setw(15) << acc_no << setw(15) << acc_type << setw(15) << acc_status << setw(15) << balance << setw(15) << interest_rate << setw(20) << holder.name << setw(address_len + 1) << holder.address << setw(15) << holder.phone << setw(20) << holder.email << endl;
        
        //dont continue till user presses enter
        std::cout << "Press enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }

    void credit(float am)
    {
        balance += am;
        // save account to file
        string contents = to_string(acc_no) + "," + acc_type + "," + acc_status + "," + to_string(get_balance()) + "," + to_string(interest_rate) + "," + holder.name + "," + holder.address + "," + holder.phone + "," + holder.email;
        int line_no = getAccountLineNo(acc_no);
        fileOps.writeFileOverwriteLine(ACCOUNT_FILE, contents, line_no);
    }

    void debit(float am)
    {
        balance -= am;
        // save account to file
        string contents = to_string(acc_no) + "," + acc_type + "," + acc_status + "," + to_string(balance) + "," + to_string(interest_rate) + "," + holder.name + "," + holder.address + "," + holder.phone + "," + holder.email;
        int line_no = getAccountLineNo(acc_no);
        fileOps.writeFileOverwriteLine(ACCOUNT_FILE, contents, line_no);
    }

    void load_account(string line)
    {
        stringstream ss(line);
        string item;
        getline(ss, item, ',');
        acc_no = stoi(item);
        getline(ss, acc_type, ',');
        getline(ss, acc_status, ',');
        getline(ss, item, ',');
        balance = stof(item);
        getline(ss, item, ',');
        interest_rate = stof(item);
        getline(ss, holder.name, ',');
        getline(ss, holder.address, ',');
        getline(ss, holder.phone, ',');
        getline(ss, holder.email, ',');
    }

    void edit_account()
    {
        system("clear");
        std::cout << "Enter new account holder name: ";
        std::cin.ignore();
        getline(std::cin, holder.name);
        std::cout << "Enter new account holder address: ";
        getline(std::cin, holder.address);
        std::cout << "Enter new account holder phone number: ";
        getline(std::cin, holder.phone);
        std::cout << "Enter new account holder email: ";
        getline(std::cin, holder.email);

        // save account to file
        string contents = to_string(acc_no) + "," + acc_type + "," + acc_status + "," + to_string(balance) + "," + to_string(interest_rate) + "," + holder.name + "," + holder.address + "," + holder.phone + "," + holder.email;
        int line_no = getAccountLineNo(acc_no);
        fileOps.writeFileOverwriteLine(ACCOUNT_FILE, contents, line_no);
    }

    void delete_account() {
        acc_status = "inactive";
        // save account to file
        string contents = to_string(acc_no) + "," + acc_type + "," + acc_status + "," + to_string(balance) + "," + to_string(interest_rate) + "," + holder.name + "," + holder.address + "," + holder.phone + "," + holder.email;
        int line_no = getAccountLineNo(acc_no);
        fileOps.writeFileOverwriteLine(ACCOUNT_FILE, contents, line_no);
    }

};

class Transaction
{
protected:
    long int txn_no;
    float amount;
    string txn_date;
    string txn_time;
    string txn_status;
    string txn_type;
    long int sender_acc_no;
    long int receiver_acc_no;

public:

    Transaction()
    {
        txn_no = 0;
        amount = 0;
        txn_date = "";
        txn_time = "";
        txn_status = "";
        sender_acc_no = 0;
        receiver_acc_no = 0;
    }

    void load_transaction(string line)
    {
        stringstream ss(line);
        string item;
        getline(ss, item, ',');
        txn_no = stoi(item);
        getline(ss, txn_type, ',');
        getline(ss, item, ',');
        amount = stof(item);
        getline(ss, txn_date, ',');
        getline(ss, txn_time, ',');
        getline(ss, txn_status, ',');
        getline(ss, item, ',');
        // sender_acc_no = stoi(item);
        if (item == "null" || item == "")
        {}
        else
        {
            sender_acc_no = stoi(item);
        }
        getline(ss, item, ',');
        // receiver_acc_no = stoi(item);
        if (item == "null" || item == "")
        {}
        else
        {
            receiver_acc_no = stoi(item);
        }
    }

    void display_txn(bool clear = true)
    {
        // system("clear");
        if (clear)
        {
            system("clear");
        }
        // use iomanip to format the output in a nice tabular form with proper headings
        std::cout << left << setw(20) << "Transaction No." << setw(20) << "Transaction Type" << setw(20) << "Amount" << setw(20) << "Date" << setw(20) << "Time" << setw(20) << "Status" << setw(20) << "Sender Acc No." << setw(20) << "Receiver Acc No." << endl;
        std::cout << left << setw(20) << txn_no << setw(20) << txn_type << setw(20) << amount << setw(20) << txn_date << setw(20) << txn_time << setw(20) << txn_status << setw(20) << sender_acc_no << setw(20) << receiver_acc_no << endl;
    }

    void transaction(Account sender, Account receiver, float amount)
    {
        if (sender.get_balance() < amount)
        {
            std::cout << "Insufficient balance" << endl;
            return;
        }

        txn_date = get_current_date();
        txn_time = get_current_time();
        txn_no = randomDigits();
        sender_acc_no = sender.get_acc_no();
        receiver_acc_no = receiver.get_acc_no();

        sender.debit(amount);
        receiver.credit(amount);
        std::cout << "Transaction successful" << endl;

        // write to file
        string contents = to_string(txn_no) + "," + "transfer" + "," + to_string(amount) + "," + txn_date + "," + txn_time + "," + "success" + "," + to_string(sender_acc_no) + "," + to_string(receiver_acc_no);
        fileOps.writeFileAppend(TXNS_FILE, contents);
    }

    void withdraw(Account root, float amount)
    {
        txn_no = randomDigits();
        if (root.get_balance() < amount)
        {
            std::cout << "Insufficient balance" << endl;
            return;
        }

        txn_date = get_current_date();
        txn_time = get_current_time();

        root.debit(amount);
        std::cout << "Withdrawal successful" << endl;
        sender_acc_no = root.get_acc_no();

        // write to file
        string contents = to_string(txn_no) + "," + "withdrawal" + "," + to_string(amount) + "," + txn_date + "," + txn_time + "," + "success" + "," + to_string(sender_acc_no) + "," + "null";
        fileOps.writeFileAppend(TXNS_FILE, contents);

        //dont continue until user presses enter
        std::cout << "Press enter to continue" << endl;
        std::cin.ignore(); 
        std::cin.get();
    }

    void deposit(Account root, float amount)
    {
        txn_no = randomDigits();
        root.credit(amount);
        std::cout << "Deposit successful" << endl;
        txn_date = get_current_date();
        txn_time = get_current_time();
        receiver_acc_no = root.get_acc_no();

        // write to file
        string contents = to_string(txn_no) + "," + "deposit" + "," + to_string(amount) + "," + txn_date + "," + txn_time + "," + "success" + "," + "null" + "," + to_string(receiver_acc_no);
        fileOps.writeFileAppend(TXNS_FILE, contents);
        
        //dont continue until user presses enter
        std::cout << "Press enter to continue" << endl;
        std::cin.ignore(); //
        std::cin.get();

    }

    long int get_sender_acc_no()
    {
        return sender_acc_no;
    }

    long int get_receiver_acc_no()
    {
        return receiver_acc_no;
    }

    float get_amount()
    {
        return amount;
    }

};

// functions
string FileOperations::readFile(string filename)
{
    string line;
    string contents;
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open())
    {
        std::cout << "Could not open file" << endl;
        return "";
    }
    while (getline(fin, line))
    {
        contents += line + "\n";
    }
    fin.close();

    // decrypt contents
    contents = decrypt(contents);
    return contents;
}

string FileOperations::getLineContent(string filename, string contents, int line)
{
    if (filename == ACCOUNT_FILE)
    {
        if (line > getAccountsLen())
        {
            std::cout << "Line number exceeds number of lines in file" << endl;
            return "";
        }
    }
    else if (filename == TXNS_FILE)
    {
        if (line > getTransactionsLen())
        {
            std::cout << "Line number exceeds number of lines in file" << endl;
            return "";
        }
    }

    stringstream ss(contents);
    string lineContents;
    for (int i = 0; i < line; i++)
    {
        getline(ss, lineContents);
    }

    return lineContents;
}

void FileOperations::writeFileAppend(string filename, string contents)
{
    // encrypt contents
    contents = encrypt(contents);
    ofstream fout;
    fout.open(filename, ios::app);
    fout << contents << endl;
    fout.close();
}

void FileOperations::writeFileOverwriteLine(string filename, string content, int line)
{
    if (filename == ACCOUNT_FILE)
    {
        if (line > getAccountsLen())
        {
            std::cout << "Line number exceeds number of lines in file" << endl;
            return;
        }
    }
    else if (filename == TXNS_FILE)
    {
        if (line > getTransactionsLen())
        {
            std::cout << "Line number exceeds number of lines in file" << endl;
            return;
        }
    }
    // encrypt contents
    content = encrypt(content);

    ifstream fin(filename);
    ofstream fout(filename + ".tmp");
    string lineContents;
    int currentLine = 0;
    while (getline(fin, lineContents))
    {
        currentLine++;
        if (currentLine == line)
        {
            fout << content << endl;
        }
        else
        {
            fout << lineContents << endl;
        }
    }
    fin.close();
    fout.close();

    remove(filename.c_str());
    rename((filename + ".tmp").c_str(), filename.c_str());
}

void Account::viewTransactions(int n) {
    loadAllTransactions(false);
    //all transactions are stored in a map, named transactions
    //map<long int, Transaction> transactions; iterate through the map and display all transactions with the given account number
    int count = 0;
    for (auto itr = transactions.begin(); itr != transactions.end(); itr++)
    {
        if (itr->second.get_sender_acc_no() == acc_no || itr->second.get_receiver_acc_no() == acc_no)
        {
            itr->second.display_txn(false);
            count++;
            if (count == n)
            {
                break;
            }
        }
    }

    if (count == 0)
    {
        std::cout << "No transactions found" << endl;
    }

    //dont continue until user presses enter
    std::cout << "Press enter to continue" << endl;
    std::cin.ignore(); 
    std::cin.get();
}

string FileOperations::encrypt(string contents)
{
    string encryptedContents;
    for (int i = 0; i < contents.length(); i++)
    {
        if (contents[i] == '\n' || contents[i] == '\0' || contents[i] == ',')
        {
            encryptedContents += contents[i];
            continue;
        }
        encryptedContents += contents[i] + encryptKey;
    }
    return encryptedContents;
}

string FileOperations::decrypt(string contents)
{
    string decryptedContents;
    for (int i = 0; i < contents.length(); i++)
    {
        if (contents[i] == '\n' || contents[i] == '\0' || contents[i] == ',')
        {
            decryptedContents += contents[i];
            continue;
        }
        decryptedContents += contents[i] - encryptKey;
    }
    return decryptedContents;
}

int getAccountsLen()
{
    string contents = fileOps.readFile(ACCOUNT_FILE);
    stringstream ss(contents);
    string line;
    int count = 0;
    while (getline(ss, line))
    {
        count++;
    }
    return count;
}

int getTransactionsLen()
{
    string contents = fileOps.readFile(TXNS_FILE);
    stringstream ss(contents);
    string line;
    int count = 0;
    while (getline(ss, line))
    {
        count++;
    }
    return count;
}

int getAccountLineNo(long int acc_no)
{
    string contents = fileOps.readFile(ACCOUNT_FILE);
    stringstream ss(contents);
    string line;
    int count = 0;
    while (getline(ss, line))
    {
        count++;
        stringstream ss2(line);
        string item;
        getline(ss2, item, ',');
        if (stoi(item) == acc_no)
        {
            return count;
        }
    }
    return -1;
}

int getTransactionLineNo(long int txn_no)
{
    string contents = fileOps.readFile(TXNS_FILE);
    stringstream ss(contents);
    string line;
    int count = 0;
    while (getline(ss, line))
    {
        count++;
        stringstream ss2(line);
        string item;
        getline(ss2, item, ',');
        if (stoi(item) == txn_no)
        {
            return count;
        }
    }
    return -1;
}

long randomDigits()
{
    // Initialize the random number generator with the current time
    srand(time(0));

    // Declare a variable to store the result
    long long result = 0;

    // Loop through 8 digits
    for (int i = 0; i < 8; i++)
    {
        // Generate a random digit from 0 to 9
        int digit = rand() % 10;

        // Append the digit to the result
        result = result * 10 + digit;
    }

    // Return the result
    if (result < 0)
    {
        result *= -1;
    }

    return result;
}

Account getAccount(long int acc_no)
{
    map<long int, Account>::iterator itr;
    itr = accounts.find(acc_no);
    if (itr == accounts.end())
    {
        std::cout << "Account not found" << endl;
        return Account();
    }
    else
    {
        return itr->second; // returns the value of the key
    }
}

Transaction getTransaction(long int txn_no)
{
    map<long int, Transaction>::iterator itr;
    itr = transactions.find(txn_no);
    if (itr == transactions.end())
    {
        std::cout << "Transaction not found" << endl;
        return Transaction();
    }
    else
    {
        return itr->second; // returns the value of the key
    }
}

/// @brief Loads all accounts from the file into the accounts map
void loadAllAccounts(bool load)
{
    // std::cout << "Fetching accounts from file....." << endl;
    if (load) {
        std::cout << "Fetching accounts from file....." << endl;
    }
    int totAccounts = getAccountsLen();
    for (int i = 0; i < totAccounts; i++)
    {
        string line = fileOps.getLineContent(ACCOUNT_FILE, fileOps.readFile(ACCOUNT_FILE), i + 1);
        // ONLY fetch account number to store in map
        stringstream ss(line);
        long int acc_no = 0;
        string temp;
        getline(ss, temp, ',');
        acc_no = stol(temp);
        // std::cout << acc_no << endl;
        Account a;
        a.load_account(fileOps.getLineContent(ACCOUNT_FILE, fileOps.readFile(ACCOUNT_FILE), i + 1));
        accounts[acc_no] = a;
    }

    if (load) {
        std::cout << "Fetched " << accounts.size() << " accounts" << endl;
        for (int i = 0; i < 1000000000; i++){}; // for loop for delay
        system("clear");
    }
}

void loadAllTransactions(bool load)
{
    if (load)
    {
        std::cout << "Fetching transactions from file...." << endl;
    }
    //clear the map
    transactions.clear();
    int totTransactions = getTransactionsLen();
    for (int i = 0; i < totTransactions; i++)
    {
        //load the transactions from file into the map
        string line = fileOps.getLineContent(TXNS_FILE, fileOps.readFile(TXNS_FILE), i + 1);
        // ONLY fetch transaction number to store in map
        stringstream ss(line);
        long int txn_no = 0;
        string temp;
        getline(ss, temp, ',');
        txn_no = stol(temp);
        // std::cout << txn_no << endl;
        Transaction t;
        t.load_transaction(line);
        transactions[txn_no] = t;
    }
    if (load) {
        std::cout << "Fetched " << transactions.size() << " transactions" << endl;
        for (int i = 0; i < 1000000000; i++)
        {}; // for loop for delay
        system("clear");
    }
}

string get_current_date()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string date = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
    return date;
}

string get_current_time()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string time = to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
    return time;
}

int main()
{
    loadAllTransactions(true); // load all transactions in the map
    loadAllAccounts();     // load all accounts in the map
    MAIN_MENU:
    // start the main menu
    std::cout << "Welcome to the bank" << endl;
    // std::cout << "Enter your account number: ";
    std::cout << "============================" << endl; // divider
    std::cout << "What would you like to do?" << endl;
    std::cout << "1. Create Account" << endl;
    std::cout << "2. Login" << endl;
    std::cout << "3. Exit" << endl;
    std::cout << "============================" << endl; // divider
    int c;
    std::cin >> c;
    Account acc;

    switch (c)
    {
    case 1:
        acc.register_acc();
        //load to map
        accounts[acc.get_acc_no()] = acc;
        goto MAIN_MENU;
        break;
    case 2:
        std::cout << "Enter your account number: ";
        long int acc_no;
        std::cin >> acc_no;
        acc = getAccount(acc_no);
        break;
    case 3:
        std::cout << "Exiting...." << endl;
        return 0;
    
    default:
        break;
    }    

    if (acc.get_acc_no() == 0)
    {
        goto MAIN_MENU;
    } else if (acc.get_acc_no() == -1) {
        goto MAIN_MENU;
    }
    else
    {
        std::cout << "Account found" << endl;
        acc.display_account();
    MENU:
        loadAllAccounts();
        loadAllTransactions();
        acc = getAccount(acc.get_acc_no());
        system("clear");
        std::cout << "============================" << endl; // divider
        std::cout << "What would you like to do?" << endl;
        std::cout << "1. Deposit" << endl;
        std::cout << "2. Withdraw" << endl;
        std::cout << "3. Transfer" << endl;
        std::cout << "4. View Recent Transactions" << endl;
        std::cout << "5. Edit Account Details" << endl;
        std::cout << "6. Delete Account" << endl;
        std::cout << "7. See Account Details" << endl; // "See Account Details
        std::cout << "8. Exit" << endl;
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        if (choice > 8 || choice < 1)
        {
            std::cout << "Invalid choice" << endl;
            std::cout << "Press any key to continue" << endl;
            std::cin.ignore();
            std::cin.get();
            system("clear");
            goto MENU;
        }

        Transaction tx;
        SUBMENU:
        switch (choice)
        {
        case 1:
            float amount;
            std::cout << "Enter amount to deposit: ";
            std::cin >> amount;
            tx.deposit(acc, amount);
            goto MENU;
            break;
        case 2:
            std::cout << "Enter amount to withdraw: ";
            std::cin >> amount;
            tx.withdraw(acc, amount);
            goto MENU;
            break;
        case 3:
            long int receiver_acc_no;
            std::cout << "Enter receiver's account number: ";
            std::cin >> receiver_acc_no;
            //check if account exists
            if (getAccount(receiver_acc_no).get_acc_no() == 0)
            {
                goto SUBMENU;
            }
            std::cout << "Enter amount to transfer: ";
            std::cin >> amount;
            //check if amount is valid
            if (amount > acc.get_balance())
            {
                std::cout << "Insufficient balance" << endl;
                goto SUBMENU;
            }
            if (amount <= 0)
            {
                std::cout << "Invalid amount" << endl;
                goto SUBMENU;
            }
            if (acc.get_acc_no() == receiver_acc_no)
            {
                std::cout << "Cannot transfer to same account" << endl;
                goto SUBMENU;
            }
            tx.transaction(acc, getAccount(receiver_acc_no), amount);
            goto MENU;
            break;
        case 4:
            std::cout << "Enter number of transactions to view: ";
            int n;
            std::cin >> n;
            acc.viewTransactions(n);
            goto MENU;
            break;
        case 5:
            acc.edit_account();
            goto MENU;
            break;
        case 6:
            std::cout << "Are you sure you want to delete your account? (y/n): ";
            char ch;
            std::cin >> ch;
            if (ch == 'y' || ch == 'Y')
            {
                acc.delete_account();
                std::cout << "Account deleted successfully" << endl;
                return 0;
            }
            else
            {
                goto SUBMENU;
            }
            break;
        case 7:
            acc.display_account();
            goto MENU;
            break;
        case 8:
            std::cout << "Thank you for using our services" << endl;
            return 0;
        default:
            break;
        }
    }
    return 0;
}
