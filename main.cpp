#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include "buyer.h"
#include "seller.h"
#include "bank_customer.h"
using namespace std;

vector<buyer> buyers;
vector<seller> sellers;
vector<bank_customer> bankCustomers;

/// *receipt
void generateReceipt(const string& buyerID, const string& sellerID,
                     const string& itemName, int qty, double total, const string& type) {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", localtime(&now));

    string filename = "receipt_" + buyerID + "_" + buf + ".txt";
    ofstream receipt(filename);
    if (!receipt.is_open()) {
        cerr << "Error creating receipt file!\n";
        return;
    }

    receipt << "==============================\n";
    receipt << "        TRANSACTION RECEIPT   \n";
    receipt << "==============================\n";
    receipt << "Date       : " << buf << "\n";
    receipt << "Buyer ID   : " << buyerID << "\n";
    receipt << "Seller ID  : " << sellerID << "\n";
    receipt << "Transaction: " << type << "\n";
    receipt << "------------------------------\n";
    receipt << "Item Name  : " << itemName << "\n";
    receipt << "Quantity   : " << qty << "\n";
    receipt << "Total Cost : $" << total << "\n";
    receipt << "==============================\n";
    receipt << "Thank you for using our system!\n";
    receipt.close();

    cout << "Receipt generated: " << filename << "\n";
}

/// *load *save *buyer
void saveBuyers(const vector<buyer>& buyers) {
    ofstream file("buyers.txt");
    if (!file.is_open()) {
        cerr << "Error saving buyers!\n";
        return;
    }
    for (const auto& b : buyers)
        file << b.getId() << '|' << b.getName() << '\n';
    file.close();
}

void loadBuyers(vector<buyer>& buyers) {
    ifstream file("buyers.txt");
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name;
        getline(ss, id, '|');
        getline(ss, name, '|');
        buyers.push_back(buyer(id, name));
    }
    file.close();
}

/// *save *seller
void saveSellers(const vector<seller>& sellers) {
    ofstream file("sellers.txt");
    if (!file.is_open()) {
        cerr << "Error saving sellers!\n";
        return;
    }
    for (const auto& s : sellers) {
        file << s.getId() << '|' << s.getName() << '|';
        const auto& items = s.getInventory();
        for (size_t i = 0; i < items.size(); ++i) {
            file << items[i].getId() << ':'
                 << items[i].getName() << ':'
                 << items[i].getStock() << ':'
                 << items[i].getPrice();
            if (i < items.size() - 1) file << ',';
        }
        file << '\n';
    }
    file.close();
}

void loadSellers(vector<seller>& sellers) {
    ifstream file("sellers.txt");
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, itemsData;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, itemsData, '|');
        seller s(id, name);

        stringstream itemStream(itemsData);
        string itemToken;
        while (getline(itemStream, itemToken, ',')) {
            stringstream is(itemToken);
            string itemId, itemName, qtyStr, priceStr;
            getline(is, itemId, ':');
            getline(is, itemName, ':');
            getline(is, qtyStr, ':');
            getline(is, priceStr, ':');
            if (!itemId.empty()) {
                int qty = stoi(qtyStr);
                double price = stod(priceStr);
                s.addNewItem(Item(itemId, itemName, qty, price));
            }
        }
        sellers.push_back(s);
    }
    file.close();
}

/// *save *bank *customer
void saveBankCustomers(const vector<bank_customer>& customers) {
    ofstream file("bank_customers.txt");
    if (!file.is_open()) return;
    for (auto& c : customers)
        file << c.getId() << '|' << c.getName() << '|' << c.getBalance() << '\n';
    file.close();
}

void loadBankCustomers(vector<bank_customer>& customers) {
    ifstream file("bank_customers.txt");
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, balStr;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, balStr, '|');
        double balance = stod(balStr);
        customers.push_back(bank_customer(id, name, balance));
    }
    file.close();
}

void logTransaction(const string& buyerID, const string& sellerID, const string& itemID,
                    const string& itemName, int qty, double totalCost, const string& type = "PURCHASE") {
    ofstream file("transactions.txt", ios::app);
    if (!file.is_open()) return;
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    file << buyerID << '|' << sellerID << '|' << itemID << '|' 
         << itemName << '|' << qty << '|' << totalCost << '|' << buf << '|' << type << '\n';
    file.close();
}

/// *buyer *transaction
struct Transaction {
    string buyerID, sellerID, itemID, itemName, date, type;
    int qty;
    double total;
};

vector<Transaction> loadBuyerTransactions(const string& buyerID) {
    ifstream file("transactions.txt");
    vector<Transaction> tx;
    if (!file.is_open()) return tx;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Transaction t;
        string qtyStr, totalStr;
        getline(ss, t.buyerID, '|');
        getline(ss, t.sellerID, '|');
        getline(ss, t.itemID, '|');
        getline(ss, t.itemName, '|');
        getline(ss, qtyStr, '|');
        getline(ss, totalStr, '|');
        getline(ss, t.date, '|');
        getline(ss, t.type, '|');
        if (t.buyerID == buyerID) {
            t.qty = stoi(qtyStr);
            t.total = stod(totalStr);
            tx.push_back(t);
        }
    }
    file.close();
    return tx;
}

void viewBuyerTransactions(const string& buyerID) {
    auto tx = loadBuyerTransactions(buyerID);
    if (tx.empty()) {
        cout << "No transactions found.\n";
        return;
    }
    cout << "\n--- Purchase History ---\n";
    for (size_t i = 0; i < tx.size(); ++i)
        cout << i + 1 << ". " << tx[i].itemName << " (x" << tx[i].qty << ") | $" << tx[i].total
             << " | Seller: " << tx[i].sellerID << " | Date: " << tx[i].date
             << " | Type: " << tx[i].type << "\n";
}

/// *refund
void processRefund(buyer& b) {
    auto tx = loadBuyerTransactions(b.getId());
    if (tx.empty()) {
        cout << "No transactions available for refund.\n";
        return;
    }

    cout << "\n--- Refundable Transactions ---\n";
    for (size_t i = 0; i < tx.size(); ++i)
        if (tx[i].type == "PURCHASE")
            cout << i + 1 << ". " << tx[i].itemName << " | Qty: " << tx[i].qty
                 << " | Total: $" << tx[i].total << " | Seller: " << tx[i].sellerID << "\n";

    cout << "Enter transaction number to refund (0 to cancel): ";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 0 || choice > (int)tx.size()) return;

    Transaction t = tx[choice - 1];
    if (t.type != "PURCHASE") {
        cout << "This transaction cannot be refunded.\n";
        return;
    }

    auto buyerBank = find_if(bankCustomers.begin(), bankCustomers.end(),
                             [&](const bank_customer& c){ return c.getId() == b.getId(); });
    auto sellerBank = find_if(bankCustomers.begin(), bankCustomers.end(),
                             [&](const bank_customer& c){ return c.getId() == t.sellerID; });

    if (buyerBank == bankCustomers.end() || sellerBank == bankCustomers.end()) {
        cout << "Bank accounts missing. Refund failed.\n";
        return;
    }

    if (sellerBank->getBalance() < t.total) {
        cout << "Seller has insufficient funds to process refund.\n";
        return;
    }

    sellerBank->withdraw(t.total);
    buyerBank->deposit(t.total);

    auto it = find_if(sellers.begin(), sellers.end(),
                      [&](const seller& s){ return s.getId() == t.sellerID; });
    if (it != sellers.end()) {
        auto& inv = it->getInventory();
        auto itemIt = find_if(inv.begin(), inv.end(),
                              [&](const Item& i){ return i.getId() == t.itemID; });
        if (itemIt != inv.end()) itemIt->increaseStock(t.qty);
    }

    logTransaction(t.buyerID, t.sellerID, t.itemID, t.itemName, t.qty, t.total, "REFUND");
    generateReceipt(t.buyerID, t.sellerID, t.itemName, t.qty, t.total, "REFUND");
    saveSellers(sellers);
    saveBankCustomers(bankCustomers);

    cout << "Refund processed successfully. $" << t.total << " returned to your account.\n";
}

/// *buyer *menu
void buyerMenu(buyer& b) {
    int choice;
    do {
        cout << "\n=== BUYER MENU (" << b.getName() << ") ===\n";
        cout << "1. View All Sellers\n";
        cout << "2. Browse & Purchase\n";
        cout << "3. View Purchase History\n";
        cout << "4. Request Refund\n";
        cout << "5. Return to Main Menu\n";
        cout << "Choose: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "\n--- Sellers ---\n";
            for (const auto& s : sellers)
                cout << "ID: " << s.getId() << " | Name: " << s.getName() << "\n";
        } 
        else if (choice == 2) {
            string sellerId;
            cout << "Enter Seller ID: ";
            getline(cin, sellerId);
            auto it = find_if(sellers.begin(), sellers.end(),
                              [&](const seller& s){ return s.getId() == sellerId; });
            if (it == sellers.end()) { cout << "Seller not found.\n"; continue; }

            seller& s = *it;
            s.showInventory();
            string itemId;
            cout << "Enter Item ID to buy (0 to cancel): ";
            getline(cin, itemId);
            if (itemId == "0") continue;

            auto& inv = s.getInventory();
            auto itemIt = find_if(inv.begin(), inv.end(),
                                  [&](const Item& i){ return i.getId() == itemId; });
            if (itemIt == inv.end()) { cout << "Item not found.\n"; continue; }

            Item& item = *itemIt;
            int qty;
            cout << "Enter quantity: ";
            cin >> qty; cin.ignore();
            if (qty > item.getStock()) { cout << "Not enough stock.\n"; continue; }

            double total = qty * item.getPrice();
            auto buyerBank = find_if(bankCustomers.begin(), bankCustomers.end(),
                                    [&](const bank_customer& c){ return c.getId() == b.getId(); });
            auto sellerBank = find_if(bankCustomers.begin(), bankCustomers.end(),
                                    [&](const bank_customer& c){ return c.getId() == s.getId(); });

            if (buyerBank == bankCustomers.end()) { cout << "Buyer has no bank account.\n"; continue; }
            if (sellerBank == bankCustomers.end()) { cout << "Seller has no bank account.\n"; continue; }

            if (buyerBank->getBalance() < total) { cout << "Insufficient funds.\n"; continue; }

            buyerBank->withdraw(total);
            sellerBank->deposit(total);
            item.reduceStock(qty);
            saveSellers(sellers);
            saveBankCustomers(bankCustomers);
            logTransaction(b.getId(), s.getId(), item.getId(), item.getName(), qty, total);
            generateReceipt(b.getId(), s.getId(), item.getName(), qty, total, "PURCHASE");
            cout << "Purchase successful!\n";
        }
        else if (choice == 3) {
            viewBuyerTransactions(b.getId());
        }
        else if (choice == 4) {
            processRefund(b);
        }

    } while (choice != 5);
}

/// *main menu *menu
int main() {
    loadBuyers(buyers);
    loadSellers(sellers);
    loadBankCustomers(bankCustomers);

    int choice;
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Register Buyer\n";
        cout << "2. Register Seller\n";
        cout << "3. Buyer Login\n";
        cout << "4. Seller Login\n";
        cout << "5. Bank Services\n";
        cout << "6. Exit\n";
        cout << "Choose: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string id, name;
            cout << "Enter Buyer ID: "; getline(cin, id);
            cout << "Enter Name: "; getline(cin, name);
            buyers.push_back(buyer(id, name));
            cout << "Buyer registered!\n";
        } 
        else if (choice == 2) {
            string id, name;
            cout << "Enter Seller ID: "; getline(cin, id);
            cout << "Enter Name: "; getline(cin, name);
            sellers.push_back(seller(id, name));
            cout << "Seller registered!\n";
        } 
        else if (choice == 3) {
            string id;
            cout << "Enter Buyer ID: "; getline(cin, id);
            auto it = find_if(buyers.begin(), buyers.end(),
                             [&](const buyer& b){ return b.getId() == id; });
            if (it != buyers.end()) buyerMenu(*it);
            else cout << "Buyer not found.\n";
        } 
        else if (choice == 4) {
            string id;
            cout << "Enter Seller ID: "; getline(cin, id);
            auto it = find_if(sellers.begin(), sellers.end(),
                             [&](const seller& s){ return s.getId() == id; });
            if (it != sellers.end()) sellerMenu(*it);
            else cout << "Seller not found.\n";
        } 
        else if (choice == 5) {
            bankMenu(bankCustomers);
        } 
        else if (choice == 6) {
            saveBuyers(buyers);
            saveSellers(sellers);
            saveBankCustomers(bankCustomers);
            cout << "All data saved. Goodbye!\n";
        }
    } while (choice != 6);

    return 0;
}
