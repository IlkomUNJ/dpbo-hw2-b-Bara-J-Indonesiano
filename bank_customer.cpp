#include "bank_customer.h"
#include <iostream>

void BankCustomer::addBalance(double amount) {
    this->balance += amount;
    transactions.push_back("Deposit: +" + to_string(amount));
}

bool BankCustomer::withdrawBalance(double amount) {
    if (amount > this->balance) {
        std::cout << "Rejected: Insufficient funds!" << std::endl;
        transactions.push_back("Withdraw failed: -" + to_string(amount));
        return false;
    }
    this->balance -= amount;
    transactions.push_back("Withdraw: -" + to_string(amount));
    return true;
}

void BankCustomer::printInfo() const {
    std::cout << "Customer Name: " << this->name << std::endl;
    std::cout << "Customer ID: " << this->id << std::endl;
    std::cout << "Balance: $" << this->balance << std::endl;
    std::cout << "Address: " << this->address << std::endl;
    std::cout << "Phone: " << this->phone << std::endl;
    std::cout << "Email: " << this->email << std::endl;
}

void BankCustomer::showTransactions() const {
    std::cout << "Transaction History:" << std::endl;
    if (transactions.empty()) {
        std::cout << "  (none)" << std::endl;
        return;
    }
    for (const auto &t : transactions) {
        std::cout << "  " << t << std::endl;
    }
}
