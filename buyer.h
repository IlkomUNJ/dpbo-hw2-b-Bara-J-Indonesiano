#ifndef BUYER_H
#define BUYER_H

#include <cstddef>
#include <string>
#include <vector>
#include "bank_customer.h"
#include "item.h"

using namespace std;

struct Invoice {
    int id;
    vector<Item> items;
    double total;
    string status;
    int sellerId;
};

class buyer {
private:
    int id;
    string name;
    bank_customer *account;

    /// *profile
    string address;
    string phone;
    string email;

    vector<Item> cart;
    vector<Invoice> invoices;

public:
    buyer() : id(0), name(""), account(nullptr) {}
    buyer(int id, const string& name, bank_customer *account0 = nullptr)
        : id(id), name(name), account(account0), address(""), phone(""), email("") {}

    int getId() const { return id; }
    string getName() const { return name; }
    bank_customer* getAccount() { return account; }

    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }

    void setProfile(const string& addr, const string& ph, const string& em) {
        address = addr;
        phone = ph;
        email = em;
    }

    void linkBankAccount(bank_customer *acc) { account = acc; }

    void addToCart(const Item &it) { cart.push_back(it); }
    void removeFromCartByIndex(size_t idx) {
        if (idx < cart.size()) cart.erase(cart.begin() + idx);
    }
    const vector<Item>& getCart() const { return cart; }
    void clearCart() { cart.clear(); }

    /// *invoice
    void addInvoice(const Invoice &inv) { invoices.push_back(inv); }
    vector<Invoice>& getInvoices() { return invoices; }

    void printProfile() const {
        cout << "Buyer ID: " << id << " | Name: " << name << endl;
        cout << "Address: " << address
             << " | Phone: " << phone
             << " | Email: " << email << endl;

        if (account)
            account->printInfo();
        else
            cout << "No linked bank account.\n";
    }
};

#endif
