#include "Item.h"

using namespace std;

Item::Item() {
    itemId = "";
    name = "";
    category = "";
    quantity = 0;
    unitPrice = 0.0;
    location = "";
}

Item::Item(string id, string n, string cat, int qty, double price, string loc) {
    itemId = id;
    name = n;
    category = cat;
    quantity = qty;
    unitPrice = price;
    location = loc;
}

string Item::getItemId() { return itemId; }
string Item::getName() { return name; }
string Item::getCategory() { return category; }
int Item::getQuantity() { return quantity; }
double Item::getUnitPrice() { return unitPrice; }
string Item::getLocation() { return location; }

void Item::setItemId(string id) { itemId = id; }
void Item::setName(string n) { name = n; }
void Item::setCategory(string cat) { category = cat; }
void Item::setQuantity(int qty) { quantity = qty; }
void Item::setUnitPrice(double price) { unitPrice = price; }
void Item::setLocation(string loc) { location = loc; }

void Item::display() {
    cout << itemId << "\t"
         << (name.length() > 20 ? name.substr(0, 17) + "..." : name) << "\t\t"
         << category << "\t\t"
         << quantity << "\t$"
         << unitPrice << "\t"
         << location << "\n";
}

string Item::serialize() {
    return itemId + "," + name + "," + category + "," + to_string(quantity) + "," + to_string(unitPrice) + "," + location;
}

Item Item::deserialize(string line) {
    size_t p1 = line.find(',');
    if (p1 == string::npos) return Item();
    size_t p2 = line.find(',', p1 + 1);
    if (p2 == string::npos) return Item();
    size_t p3 = line.find(',', p2 + 1);
    if (p3 == string::npos) return Item();
    size_t p4 = line.find(',', p3 + 1);
    if (p4 == string::npos) return Item();
    size_t p5 = line.find(',', p4 + 1);
    if (p5 == string::npos) return Item();

    string id = line.substr(0, p1);
    string n = line.substr(p1 + 1, p2 - p1 - 1);
    string cat = line.substr(p2 + 1, p3 - p2 - 1);
    string qtyStr = line.substr(p3 + 1, p4 - p3 - 1);
    string priceStr = line.substr(p4 + 1, p5 - p4 - 1);
    string loc = line.substr(p5 + 1);

    int qty = 0;
    double price = 0.0;
    try {
        if (!qtyStr.empty()) qty = stoi(qtyStr);
    } catch (...) { qty = 0; }
    try {
        if (!priceStr.empty()) price = stod(priceStr);
    } catch (...) { price = 0.0; }

    return Item(id, n, cat, qty, price, loc);
}
