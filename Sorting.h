#ifndef SORTING_H
#define SORTING_H

#include <iostream>
#include <string>
#include "Item.h"

using namespace std;

class Sorting {
private:
    static bool compareItems(Item a, Item b, int criterion);
    static void merge(Item arr[], int left, int mid, int right, int criterion);
    static int partition(Item arr[], int low, int high, int criterion);

public:
    enum Criterion {
        BY_ID = 0,
        BY_PRICE = 1,
        BY_QUANTITY = 2,
        BY_NAME = 3
    };

    static void mergeSort(Item arr[], int left, int right, int criterion = BY_ID);
    static void quickSort(Item arr[], int low, int high, int criterion = BY_ID);

    static int linearSearchById(Item arr[], int size, string itemId);
    static void linearSearch(Item arr[], int size, string keyword);
    static int binarySearchById(Item arr[], int size, string itemId);
};

#endif
