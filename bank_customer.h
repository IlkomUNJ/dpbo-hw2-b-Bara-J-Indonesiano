#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <iostream>
#include <string>
using namespace std;

class bank_customer {
private:
    string id;
    string name;
    double balance;

public:
    bank_customer(string i = "", string n = "", double b = 0.0)
        : id(i), name(n), balance(b) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    // Basic banking operations
    void deposit(double amount) { balance += amount; }
    void withdraw(double amount) { balance -= amount; }

    void printInfo() const {
        cout << "Bank Customer: " << name
             << " | ID: " << id
             << " | Balance: $" << balance << endl;
    }
};

#endif
