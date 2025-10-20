#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

class Item {
private:
    string id;
    string name;
    int stock;
    double price;

public:
    Item(string id = "", string name = "", int stock = 0, double price = 0.0)
        : id(id), name(name), stock(stock), price(price) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    int getStock() const { return stock; }
    double getPrice() const { return price; }

    // Modify stock
    void reduceStock(int qty) { stock -= qty; }
    void increaseStock(int qty) { stock += qty; }
};

#endif
