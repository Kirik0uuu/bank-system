#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Account {
    private:
        string owner;
        double balance;
        int ID;
    public:
        Account(string newOwner, int newId, double newBalance = 0) {
            owner = newOwner;
            ID = newId;
            balance = newBalance;
        }
        int getID() {
            return ID;
        }
        string getOwner() {
            return owner;
        }
        double getBalance() {
            return balance;
        }
        void deposit(double amount) {
            if (amount <= 0) {
                cout << "Invalid amount" << endl;
            } else {
                balance += amount;
                cout << "Deposit done. New balance: " << balance << endl;
            }
        }
        bool withdraw(double amount) {
            if (amount <= 0) {
                cout << "Invalid amount" << endl;
                return false;
            } else if (amount > balance) {
                cout << "Withdrawal amount exceeds balance!" << endl;
                return false;
            } else {
                balance -= amount;
                return true;
            }
        }
        void save(ofstream &bank) {
            bank << ID << " " << balance << " " << owner << endl;
        }
};

int findAccount(vector<Account> &accounts, int id) {
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].getID() == id) {
            return i;
        }
    }
    return -1;
}

void loadAccounts(vector<Account> &accounts, int &nextID) {
    ifstream bank("bankfile.txt");
    if (!bank) {
        return;
    }
    int id;
    double balance;
    string name;
    while (bank >> id >> balance) {
        bank.ignore();
        getline(bank, name);
        accounts.push_back(Account(name, id, balance));
        if (id >= nextID) {
            nextID = id + 1;
        }
    }
    bank.close();
    cout << "Accounts loaded: " << accounts.size() << endl;
}

void saveAccounts(vector<Account> &accounts) {
    ofstream bank("bankfile.txt");
    for (int i = 0; i < accounts.size(); i++) {
        accounts[i].save(bank);
    }
    bank.close();
    cout << "Saving accounts..." << endl;
}

void viewAccounts(vector<Account> &accounts) {
    if (accounts.size() == 0) {
        cout << "No accounts yet" << endl;
        return;
    }
    cout << "-------------------------------------------" << endl;
    for (int i = 0; i < accounts.size(); i++) {
        cout << "ID: " << accounts[i].getID()
             << " | Owner: " << accounts[i].getOwner()
             << " | Balance: " << accounts[i].getBalance() << endl;
    }
    cout << "-------------------------------------------" << endl;
}

int main() {
    vector<Account> accounts;
    int nextID = 1000;
    int choice = 0;

    cout << "*******************************************\nWELCOME TO OUR BANK\n*******************************************\n";

    loadAccounts(accounts, nextID);

    while (choice != 8) {
        cout << "1. Create account\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Check balance\n6. View all accounts\n7. Save accounts\n8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            cout << "Enter owner name: ";
            cin.ignore();
            getline(cin, name);
            Account newAccount(name, nextID);
            accounts.push_back(newAccount);
            cout << "Your account has been created: " << name << endl;
            cout << "Your Id number is: " << nextID << endl;
            nextID += 1;
            cout<<" \n";
        }
        else if (choice == 2) {
            int id;
            double amount;
            cout << "Enter your ID: ";
            cin >> id;
            int index = findAccount(accounts, id);
            if (index == -1) {
                cout << "Account not found" << endl;
            } else {
                cout << "Enter amount to deposit: ";
                cin >> amount;
                accounts[index].deposit(amount);
            }
        }
        else if (choice == 3) {
            int id;
            double amount;
            cout << "Enter your ID: ";
            cin >> id;
            int index = findAccount(accounts, id);
            if (index == -1) {
                cout << "Account not found" << endl;
            } else {
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                if (accounts[index].withdraw(amount)) {
                    cout << "Withdrawal done. New balance: " << accounts[index].getBalance() << endl;
                    cout<<" \n";
                }
            }
        }
        else if (choice == 4) {
            int fromId, toId;
            double amount;
            cout << "Enter your ID: ";
            cin >> fromId;
            cout << "Enter the recipient ID: ";
            cin >> toId;
            int fromIndex = findAccount(accounts, fromId);
            int toIndex = findAccount(accounts, toId);
            if (fromIndex == -1 || toIndex == -1) {
                cout << "Account not found" << endl;
            } else if (fromIndex == toIndex) {
                cout << "You cannot transfer to yourself" << endl;
            } else {
                cout << "Enter the amount to transfer: ";
                cin >> amount;
                if (accounts[fromIndex].withdraw(amount)) {
                    accounts[toIndex].deposit(amount);
                    cout << "Transfer done" << endl;
                    cout<<" \n";
                }
            }
        }
        else if (choice == 5) {
            int id;
            cout << "Enter your ID: ";
            cin >> id;
            int index = findAccount(accounts, id);
            if (index == -1) {
                cout << "Account not found" << endl;
            } else {
                cout << accounts[index].getOwner() << ", your balance is: " << accounts[index].getBalance() << endl;
            }
        }
        else if (choice == 6) {
            viewAccounts(accounts);
        }
        else if (choice == 7) {
            saveAccounts(accounts);
        }
        else if (choice == 8) {
            saveAccounts(accounts);
            cout << "Exiting.....\nThankyou for trusting our bank" << endl;
        }
        else {
            cout << "Invalid choice" << endl;
            cout<<" \n";
        }
    }

    return 0;
}