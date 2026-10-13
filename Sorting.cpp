#include "Sorting.h"

using namespace std;

static string toLowerStr(string s) {
    string res = s;
    for (size_t i = 0; i < res.length(); ++i) {
        if (res[i] >= 'A' && res[i] <= 'Z') {
            res[i] = res[i] + ('a' - 'A');
        }
    }
    return res;
}

bool Sorting::compareItems(Item a, Item b, int criterion) {
    if (criterion == BY_PRICE) {
        return a.getUnitPrice() < b.getUnitPrice();
    }
    if (criterion == BY_QUANTITY) {
        return a.getQuantity() < b.getQuantity();
    }
    if (criterion == BY_NAME) {
        return toLowerStr(a.getName()) < toLowerStr(b.getName());
    }
    return a.getItemId() < b.getItemId();
}

void Sorting::merge(Item arr[], int left, int mid, int right, int criterion) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Item* L = new Item[n1];
    Item* R = new Item[n2];

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareItems(L[i], R[j], criterion)) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void Sorting::mergeSort(Item arr[], int left, int right, int criterion) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, criterion);
        mergeSort(arr, mid + 1, right, criterion);
        merge(arr, left, mid, right, criterion);
    }
}

int Sorting::partition(Item arr[], int low, int high, int criterion) {
    Item pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (compareItems(arr[j], pivot, criterion)) {
            i++;
            Item temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    Item temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

void Sorting::quickSort(Item arr[], int low, int high, int criterion) {
    if (low < high) {
        int pi = partition(arr, low, high, criterion);
        quickSort(arr, low, pi - 1, criterion);
        quickSort(arr, pi + 1, high, criterion);
    }
}

int Sorting::linearSearchById(Item arr[], int size, string itemId) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].getItemId() == itemId) {
            return i;
        }
    }
    return -1;
}

void Sorting::linearSearch(Item arr[], int size, string keyword) {
    string keyLower = toLowerStr(keyword);
    bool found = false;
    cout << "\n--- Search Results for '" << keyword << "' ---\n";
    cout << "Item ID\tName\t\t\tCategory\t\tStock\tPrice\tLocation\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < size; ++i) {
        string n = toLowerStr(arr[i].getName());
        string c = toLowerStr(arr[i].getCategory());
        string id = toLowerStr(arr[i].getItemId());

        if (n.find(keyLower) != string::npos ||
            c.find(keyLower) != string::npos ||
            id.find(keyLower) != string::npos) {
            arr[i].display();
            found = true;
        }
    }
    if (!found) {
        cout << "No matching items found.\n";
    }
}

int Sorting::binarySearchById(Item arr[], int size, string itemId) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].getItemId() == itemId) {
            return mid;
        }
        if (arr[mid].getItemId() < itemId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}
