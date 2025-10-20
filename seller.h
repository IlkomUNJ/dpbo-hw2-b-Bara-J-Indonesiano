#ifndef SELLER_H
#define SELLER_H

#include <string>
#include <vector>
#include "item.h"
using namespace std;

class seller {
private:
    string id;
    string name;
    vector<Item> inventory;

public:
    seller() {}
    seller(const string& id, const string& name)
        : id(id), name(name) {}

    string getId() const { return id; }
    string getName() const { return name; }

    vector<Item>& getInventory() { return inventory; }
    void addNewItem(const Item& item) { inventory.push_back(item); }

    void showInventory() const {
        cout << "\n--- Inventory of " << name << " ---\n";
        if (inventory.empty()) {
            cout << "No items available.\n";
            return;
        }
        for (const auto& it : inventory) {
            cout << "ID: " << it.getId()
                 << ", Name: " << it.getName()
                 << ", Stock: " << it.getStock()
                 << ", Price: " << it.getPrice() << '\n';
        }
    }
};

#endif
