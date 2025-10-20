#ifndef BANK_H
#define BANK_H

#include "bank_customer.h"
#include "bank_customer.cpp"
#include <iostream>
#include <vector>
#include <optional>

using namespace std;

class Bank {
private:
    string name;
    vector<BankCustomer> Accounts{};
    int customerCount = 0;

public:
    Bank(const string& name) : name(name), customerCount(0) {}

    BankCustomer* createAccount(int id, const string& name, double initialDeposit,
                                const string& address, const string& phone, const string& email) {
        BankCustomer bc(id, name, initialDeposit);
        bc.setAddress(address);
        bc.setPhone(phone);
        bc.setEmail(email);
        bc.logTransaction("Account created with initial deposit: " + to_string(initialDeposit));
        Accounts.push_back(bc);
        customerCount++;
        return &Accounts.back();
    }

    BankCustomer* findAccount(int id) {
        for (auto &acc : Accounts) if (acc.getId() == id) return &acc;
        return nullptr;
    }

    vector<BankCustomer>& getAccounts() { return Accounts; }
};

#endif
