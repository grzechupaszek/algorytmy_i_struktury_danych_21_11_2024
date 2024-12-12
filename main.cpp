#include <iostream>
#include <string>
using namespace std;
struct Node {
    string key;
    Node* next;

    Node(const string& keyValue) : key(keyValue), next(nullptr) {}
};

class HashTable {
private:
    Node** table; // Array of pointers to nodes
    int size;

    // Hash function
    int hashFunction(const string& key) {
        int hash = 0;
        for (char ch : key) {
            ch = tolower(ch); // Normalize to lowercase
            hash += (ch - 'a' + 1);
        }
        return hash % size;
    }

public:
    // Constructor
    HashTable(int tableSize) : size(tableSize) {
        table = new Node*[size];
        for (int i = 0; i < size; i++) {
            table[i] = nullptr; // Initialize buckets to nullptr
        }
    }

    // Destructor
    ~HashTable() {
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current) {
                Node* toDelete = current;
                current = current->next;
                delete toDelete; // Delete nodes in the linked list
            }
        }
        delete[] table; // Delete the table
    }

    // Insert a key
    void insert(const string& key) {
        int index = hashFunction(key);
        Node* newNode = new Node(key);
        if (!table[index]) {
            table[index] = newNode; // Insert as the first node
        } else {
            // Collision: Add to the front of the linked list
            newNode->next = table[index];
            table[index] = newNode;
        }
    }

    // Search for a key
    bool search(const string& key) {
        int index = hashFunction(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                return true; // Key found
            }
            current = current->next;
        }
        return false; // Key not found
    }

    // Display the hash table
    void display() {
        for (int i = 0; i < size; i++) {
            cout << i << ": ";
            Node* current = table[i];
            while (current) {
                cout << current->key << " -> ";
                current = current->next;
            }
            cout << "NULL\n";
        }
    }
};

int main() {
    HashTable hashTable(13);

    // Insert elements
    hashTable.insert("Antek");
    hashTable.insert("Piotr");
    hashTable.insert("Ola");
    hashTable.insert("Kasia");

    // Display hash table
    hashTable.display();

    // Search for elements
    cout << "Czy 'Ola' istnieje? " << (hashTable.search("Ola") ? "Tak" : "Nie") << endl;
    cout << "Czy 'Basia' istnieje? " << (hashTable.search("Basia") ? "Tak" : "Nie") << endl;

    return 0;
}
