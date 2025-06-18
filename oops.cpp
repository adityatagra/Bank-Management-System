#include <iostream>
#include <string>
using namespace std;

class Account {
protected:
    string name;
    int accountNumber;
    string accountType;
    double balance;

private:
    int pin;

public:
    Account(string n, int accNo, int p, string type) {
        name = n;
        accountNumber = accNo;
        pin = p;
        accountType = type;
        balance = 0.0;
    }

    virtual void displayInfo() = 0; 

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "✅ Deposited: $" << amount << endl;
        } else {
            cout << "❌ Invalid amount.\n";
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "✅ Withdrawn: $" << amount << endl;
        } else {
            cout << "❌ Insufficient balance or invalid amount.\n";
        }
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    bool verifyPin(int inputPin) const {
        return inputPin == pin;
    }

    friend void checkBalance(const Account& acc, int enteredPin);

    void operator>>(Account &receiver) {
        double amount;
        cout << "Enter amount to transfer: ";
        cin >> amount;

        if (amount <= balance && amount > 0) {
            balance -= amount;
            receiver.balance += amount;
            cout << "✅ Transferred $" << amount << " successfully.\n";
        } else {
            cout << "❌ Transfer failed: insufficient balance or invalid amount.\n";
        }
    }

    virtual ~Account() {}
};


void checkBalance(const Account& acc, int enteredPin) {
    if (acc.verifyPin(enteredPin)) {
        cout << "Current Balance: $" << acc.balance << endl;
    } else {
        cout << "❌ Incorrect PIN.\n";
    }
}


class SavingsAccount : public Account {
public:
    SavingsAccount(string n, int accNo, int p)
        : Account(n, accNo, p, "Savings") {}

    void displayInfo() override {
        cout << "\n[Account Type: " << accountType << "]\n";
        cout << "Name         : " << name << endl;
        cout << "Account No   : " << accountNumber << endl;
        cout << "-----------------------------\n";
    }

    ~SavingsAccount() {
        cout << "Account #" << accountNumber << " closed.\n";
    }
};


class Admin {
public:
    void showAllAccounts(SavingsAccount* accList[], int count) {
        if (count == 0) {
            cout << "\nNo accounts to display.\n";
            return;
        }
        cout << "\n--- All Bank Accounts ---\n";
        for (int i = 0; i < count; ++i) {
            accList[i]->displayInfo();
        }
    }
};

int main() {
    const int MAX = 100;
    SavingsAccount* accounts[MAX];
    int accountCount = 0;

    Admin admin;

    int choice;
    do {
        cout << "\n======= BANK MANAGEMENT SYSTEM =======\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Money\n";
        cout << "5. View Account Info\n";
        cout << "6. Check Balance\n";
        cout << "7. Admin: View All Accounts\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        int accNo, pin;
        double amount;

        switch (choice) {
            case 1: {
                string name;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Set 4-digit PIN: ";
                cin >> pin;
                accNo = 1001 + accountCount;
                accounts[accountCount] = new SavingsAccount(name, accNo, pin);
                cout << "✅ Account Created! Your Account Number: " << accNo << endl;
                accountCount++;
                break;
            }
            case 2: {
                cout << "Enter Account Number: ";
                cin >> accNo;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNo) {
                        cout << "Enter amount to deposit: ";
                        cin >> amount;
                        accounts[i]->deposit(amount);
                        break;
                    }
                }
                break;
            }
            case 3: {
                cout << "Enter Account Number: ";
                cin >> accNo;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNo) {
                        cout << "Enter amount to withdraw: ";
                        cin >> amount;
                        accounts[i]->withdraw(amount);
                        break;
                    }
                }
                break;
            }
            case 4: {
                int toAcc;
                cout << "Enter Sender Account Number: ";
                cin >> accNo;
                cout << "Enter Receiver Account Number: ";
                cin >> toAcc;
                SavingsAccount *sender = nullptr, *receiver = nullptr;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNo) sender = accounts[i];
                    if (accounts[i]->getAccountNumber() == toAcc) receiver = accounts[i];
                }
                if (sender && receiver) {
                    *sender >> *receiver;
                } else {
                    cout << "❌ Invalid account numbers.\n";
                }
                break;
            }
            case 5: {
                cout << "Enter Account Number: ";
                cin >> accNo;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNo) {
                        accounts[i]->displayInfo();
                        break;
                    }
                }
                break;
            }
            case 6: {
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter PIN: ";
                cin >> pin;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accNo) {
                        checkBalance(*accounts[i], pin);
                        break;
                    }
                }
                break;
            }
            case 7: {
                admin.showAllAccounts(accounts, accountCount);
                break;
            }
            case 8:
                cout << "👋 Exiting... Thank you for using our system!\n";
                break;
            default:
                cout << "❗ Invalid choice. Try again.\n";
        }

    } while (choice != 8);

   
    for (int i = 0; i < accountCount; ++i) {
        delete accounts[i];
    }

    return 0;
}
