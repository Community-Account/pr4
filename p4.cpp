#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BankAccount {
private:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    BankAccount(string accNo, string name, double bal) {
        accountNumber = accNo;
        accountHolderName = name;
        balance = bal;
    }

    virtual ~BankAccount() {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: " << amount << endl;
        }
    }

    virtual bool withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            cout << "Withdrawn: " << amount << endl;
            return true;
        }
        cout << "Insufficient balance!" << endl;
        return false;
    }

    double getBalance() {
        return balance;
    }

    void setBalance(double bal) {
        balance = bal;
    }

    string getAccNo() {
        return accountNumber;
    }

    virtual void calculateInterest() {
        // Base class default
    }

    virtual void displayAccountInfo() {
        cout << "\n--- Account Info ---" << endl;
        cout << "Acc No: " << accountNumber << endl;
        cout << "Holder: " << accountHolderName << endl;
        cout << "Balance: " << balance << endl;
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(string accNo, string name, double bal, double rate) 
        : BankAccount(accNo, name, bal), interestRate(rate) {}

    void calculateInterest() override {
        double interest = getBalance() * (interestRate / 100);
        deposit(interest);
        cout << "Interest added: " << interest << endl;
    }

    void displayAccountInfo() override {
        BankAccount::displayAccountInfo();
        cout << "Type: Savings | Interest Rate: " << interestRate << "%" << endl;
    }
};

class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(string accNo, string name, double bal, double limit) 
        : BankAccount(accNo, name, bal), overdraftLimit(limit) {}

    bool withdraw(double amount) override {
        if (amount > 0 && (getBalance() + overdraftLimit) >= amount) {
            setBalance(getBalance() - amount);
            cout << "Withdrawn (with overdraft): " << amount << endl;
            return true;
        }
        cout << "Overdraft limit exceeded!" << endl;
        return false;
    }

    void displayAccountInfo() override {
        BankAccount::displayAccountInfo();
        cout << "Type: Checking | Overdraft Limit: " << overdraftLimit << endl;
    }
};

class FixedDepositAccount : public BankAccount {
private:
    int term;

public:
    FixedDepositAccount(string accNo, string name, double bal, int t) 
        : BankAccount(accNo, name, bal), term(t) {}

    void calculateInterest() override {
        double rate = (term >= 12) ? 7.5 : 5.0; 
        double interest = getBalance() * (rate / 100) * (term / 12.0);
        cout << "FD Interest after " << term << " months: " << interest << endl;
    }

    void displayAccountInfo() override {
        BankAccount::displayAccountInfo();
        cout << "Type: Fixed Deposit | Term: " << term << " months" << endl;
    }
};

int main() {
    vector<BankAccount*> accounts;
    int choice;

    while (true) {
        cout << "\n--- Banking System Menu ---" << endl;
        cout << "1. Open Savings Account" << endl;
        cout << "2. Open Checking Account" << endl;
        cout << "3. Open FD Account" << endl;
        cout << "4. Deposit" << endl;
        cout << "5. Withdraw" << endl;
        cout << "6. View All Accounts (Polymorphism)" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter Choice: ";
        cin >> choice;

        if (choice == 7) break;

        if (choice >= 1 && choice <= 3) {
            string id, name;
            double bal;
            cout << "Enter Acc No, Name, Initial Balance: ";
            cin >> id >> name >> bal;

            if (choice == 1) {
                double rate;
                cout << "Enter Interest Rate: "; cin >> rate;
                accounts.push_back(new SavingsAccount(id, name, bal, rate));
            } else if (choice == 2) {
                double limit;
                cout << "Enter Overdraft Limit: "; cin >> limit;
                accounts.push_back(new CheckingAccount(id, name, bal, limit));
            } else if (choice == 3) {
                int months;
                cout << "Enter Term (months): "; cin >> months;
                accounts.push_back(new FixedDepositAccount(id, name, bal, months));
            }
        } else if (choice == 4 || choice == 5) {
            string id;
            double amt;
            cout << "Enter Account Number: "; cin >> id;
            cout << "Enter Amount: "; cin >> amt;

            for (auto acc : accounts) {
                if (acc->getAccNo() == id) {
                    if (choice == 4) acc->deposit(amt);
                    else acc->withdraw(amt);
                }
            }
        } else if (choice == 6) {
            for (auto acc : accounts) {
                acc->displayAccountInfo(); 
                acc->calculateInterest(); 
            }
        }
    }

    for (auto acc : accounts) delete acc;
}