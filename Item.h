#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

using namespace std;

class Item {
private:
    string itemId;
    string name;
    string category;
    int quantity;
    double unitPrice;
    string location;

public:
    Item();
    Item(string id, string n, string cat, int qty, double price, string loc);

    string getItemId();
    string getName();
    string getCategory();
    int getQuantity();
    double getUnitPrice();
    string getLocation();

    void setItemId(string id);
    void setName(string n);
    void setCategory(string cat);
    void setQuantity(int qty);
    void setUnitPrice(double price);
    void setLocation(string loc);

    void display();
    string serialize();
    static Item deserialize(string line);
};

#endif
